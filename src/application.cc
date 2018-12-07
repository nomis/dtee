/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2018  Simon Arlott

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "application.h"

#include <sys/types.h>
#include <sysexits.h>
#include <unistd.h>
#include <csignal>
#include <exception>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <boost/system/error_code.hpp>

#include "command_line.h"
#include "cron.h"
#include "dispatch.h"
#include "file_output.h"
#include "input.h"
#include "process.h"
#include "stream_output.h"
#include "to_string.h"

using ::boost::asio::io_service;
using ::boost::format;
using ::boost::system::error_code;
using ::std::cerr;
using ::std::exception;
using ::std::list;
using ::std::make_shared;
using ::std::shared_ptr;
using ::std::string;
using ::std::vector;

#ifdef GCOV_ENABLED
extern "C" void __gcov_flush(void);
#endif

namespace dtee {

void Application::print_error(const format &message) {
	const string line = str(format("%1%: %2%\n") % CommandLine::display_name() % message);

	cerr.clear();
	cerr.write(line.c_str(), line.length());
}

int Application::run(int argc, const char* const argv[]) {
	command_line_.parse(argc, argv);

	shared_ptr<Dispatch> output = create_dispatch();
	shared_ptr<Input> input = make_shared<Input>(command_line_, io_, output);

	bool output_ok = output->open();
	bool input_ok = input->open();
	int ret_internal = EXIT_SUCCESS;

	if (!output_ok) {
		ret_internal = EX_CANTCREAT;

		if (!command_line_.cron_mode()) {
			// If we're running from cron then we have output an error message
			// for the failed output files but must continue to execute the
			// requested command.
			return ret_internal;
		}
	}

	if (input_ok) {
		io_.notify_fork(io_service::fork_event::fork_prepare);
		pid_t pid = fork();
		if (pid > 0) {
			io_.notify_fork(io_service::fork_event::fork_parent);
			input->fork_parent(pid);
			input->start();

			bool io_ok = true;

			io_ok &= io_run();
			io_ok &= input->stop();

			if (cron_) {
				io_ok &= cron_->report();
			}

			if (!io_ok) {
				ret_internal = EX_IOERR;
			}

			int signum = process_->interrupt_signum();

			if (signum >= 0) {
				input.reset(); // Stop handling signals
#ifdef GCOV_ENABLED
				__gcov_flush(); // LCOV_EXCL_LINE
#endif
				kill(getpid(), signum);
			}

			return process_->exit_status(ret_internal);
		} else if (pid == 0) {
			io_.notify_fork(io_service::fork_event::fork_child);
			input->fork_child();
		} else {
			print_error(format("fork: %1%") % errno_to_string());
			ret_internal = EX_OSERR;
		}
	} else {
		ret_internal = EX_UNAVAILABLE;
	}

	if (ret_internal != EXIT_SUCCESS) {
		if (!command_line_.cron_mode()) {
			// If we're running from cron then we have output an error message
			// for the failed preparation to handle input but must continue to
			// execute the requested command.
			return ret_internal;
		}
	}

	// Files are opened with O_CLOEXEC so these are unnecessary
	output.reset();
	cron_.reset();

	// Stop handling signals and close all sockets
	input.reset();

	execute(command_line_.command());
	return EX_SOFTWARE;
}

shared_ptr<Dispatch> Application::create_dispatch() {
	list<shared_ptr<Output>> outputs = create_outputs();
	list<shared_ptr<ResultHandler>> result_handlers = create_result_handlers();
	return make_shared<Dispatch>(outputs, result_handlers);
}

list<shared_ptr<Output>> Application::create_outputs() {
	list<shared_ptr<Output>> outputs;
	shared_ptr<Output> original = make_shared<StreamOutput>();

	if (command_line_.cron_mode()) {
		cron_ = make_shared<Cron>(
				command_line_.command()[0],
				original);
		outputs.push_back(cron_);
	} else {
		cron_ = nullptr;
		outputs.push_back(original);
	}

	create_file_outputs(outputs, "out-overwrite", FileOutputType::STDOUT, false);
	create_file_outputs(outputs, "err-overwrite", FileOutputType::STDERR, false);
	create_file_outputs(outputs, "combined-overwrite", FileOutputType::COMBINED, false);
	create_file_outputs(outputs, "out-append", FileOutputType::STDOUT, true);
	create_file_outputs(outputs, "err-append", FileOutputType::STDERR, true);
	create_file_outputs(outputs, "combined-append", FileOutputType::COMBINED, true);

	return outputs;
}

void Application::create_file_outputs(list<shared_ptr<Output>> &outputs,
		const string &name, FileOutputType type, bool append) {
	for (const auto& filename : command_line_.list(name)) {
		outputs.push_back(make_shared<FileOutput>(filename, type, append));
	}
}

list<shared_ptr<ResultHandler>> Application::create_result_handlers() {
	list<shared_ptr<ResultHandler>> result_handlers;

	process_ = make_shared<Process>();
	result_handlers.push_back(process_);

	if (command_line_.cron_mode()) {
		result_handlers.push_back(cron_);
	}

	return result_handlers;
}

bool Application::io_run() {
	bool io_error = false;
	error_code ec;

	do {
		io_.run(ec);

		if (ec) {
			print_error(format("asio run: %1%") % ec.message());
			io_error = true;
			break;
		}
	} while (!io_.stopped());

	size_t events;
	do {
		io_.reset();
		events = io_.poll(ec);

		if (ec) {
			print_error(format("asio poll: %1%") % ec.message());
			io_error = true;
			break;
		}
	} while (events > 0);

	return !io_error;
}

void Application::execute(const vector<string> &command) {
	vector<vector<char>> args{}; //!< non-const C string copy of command strings

	for (const auto& arg : command) {
		args.emplace_back(arg.cbegin(), arg.cend() + 1);
	}

	vector<char*> argv{}; //!< C pointer array to command string copies

	argv.push_back(args[0].data());
	for (auto& arg : args) {
		argv.push_back(arg.data());
	}
	argv.push_back(nullptr);

#ifdef GCOV_ENABLED
	__gcov_flush(); // LCOV_EXCL_LINE
#endif

	errno = 0;
	execvp(argv[0], &argv.data()[1]);
	print_error(format("%1%: %2%") % argv[0] % errno_to_string());
	exit(EX_NOINPUT);
}

} // namespace dtee
