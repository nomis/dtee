/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2018,2021  Simon Arlott

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
#include "i18n.h"
#include "input.h"
#include "print_error.h"
#include "process.h"
#include "signal_handler.h"
#include "stream_output.h"

using ::boost::asio::io_service;
using ::boost::format;
using ::boost::system::error_code;
using ::std::make_shared;
using ::std::shared_ptr;
using ::std::string;
using ::std::vector;

#ifdef GCOV_ENABLED
extern "C" void __gcov_flush(void);
#endif

namespace dtee {

int Application::run(int argc, const char* const argv[]) {
	command_line_.parse(argc, argv);

	io_ = make_shared<io_service>();
	shared_ptr<Dispatch> output = create_dispatch();
	shared_ptr<Input> input = make_shared<Input>(io_, output);
	shared_ptr<SignalHandler> signal_handler = make_shared<SignalHandler>(command_line_, io_, output);

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
		io_->notify_fork(io_service::fork_event::fork_prepare);
		pid_t pid = fork();
		if (pid > 0) {
			io_->notify_fork(io_service::fork_event::fork_parent);
			signal_handler->start(pid);
			input->fork_parent();
			input->start();

			bool io_ok = true;

			io_ok &= io_run();
			io_ok &= input->stop();
			io_ok &= signal_handler->stop();

			if (cron_) {
				io_ok &= cron_->report();
			}

			if (!io_ok) {
				ret_internal = EX_IOERR;
			}

			int signum = process_->interrupt_signum();

			if (signum >= 0) {
				signal_handler.reset();
#ifdef GCOV_ENABLED
				__gcov_flush(); // LCOV_EXCL_LINE
#endif
				kill(getpid(), signum);
			}

			return process_->exit_status(ret_internal);
		} else if (pid == 0) {
			io_->notify_fork(io_service::fork_event::fork_child);
			input->fork_child();
		} else {
			print_system_error(format("fork: %1%"));
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
	signal_handler.reset();

	// This is the only way to really close all file descriptors with Boost.Asio
	// (not everything is CLOEXEC on all platforms).
	io_.reset();

	execute(command_line_.command());
	return EX_SOFTWARE;
}

shared_ptr<Dispatch> Application::create_dispatch() {
	// These must be called in this order otherwise cron_ is not set before it
	// is used as a result handler.
	auto outputs = create_outputs();
	auto result_handlers = create_result_handlers();
	return make_shared<Dispatch>(outputs, result_handlers);
}

vector<shared_ptr<Output>> Application::create_outputs() {
	vector<shared_ptr<Output>> outputs;
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

void Application::create_file_outputs(vector<shared_ptr<Output>> &outputs,
		const string &name, FileOutputType type, bool append) {
	for (const auto& filename : command_line_.list(name)) {
		outputs.push_back(make_shared<FileOutput>(filename, type, append));
	}
}

vector<shared_ptr<ResultHandler>> Application::create_result_handlers() {
	vector<shared_ptr<ResultHandler>> result_handlers;

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

	// Wait for events until the I/O service is explicitly stopped
	do {
		io_->run(ec);

		if (ec) {
			print_error(format("asio run: %1%"), ec);
			io_error = true;
			break;
		}
	} while (!io_->stopped());

	// Poll until there are no more events
	size_t events;
	do {
		io_->reset();
		events = io_->poll(ec);

		if (ec) {
			print_error(format("asio poll: %1%"), ec);
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
	print_system_error(format(_("%1%: %2%")) % argv[0]);
	exit(EX_NOINPUT);
}

} // namespace dtee
