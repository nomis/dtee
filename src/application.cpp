/*
	dtee - run a program with standard output and standard error copied to files
	Copyright 2018,2021-2022,2024-2025  Simon Arlott

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
#include <unistd.h>

#include <cerrno>
#include <csignal>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <system_error>
#include <typeinfo>
#include <vector>

// https://github.com/chriskohlhoff/asio/issues/721
#pragma GCC diagnostic push
#pragma GCC diagnostic warning "-Wshadow"
#include <boost/asio.hpp>
#pragma GCC diagnostic pop
#include <boost/core/typeinfo.hpp>
#include <boost/format.hpp>

#include "command_line.h"
#include "coverage.h"
#include "cron.h"
#include "dispatch.h"
#include "file_output.h"
#include "i18n.h"
#include "input.h"
#include "print_error.h"
#include "process.h"
#include "signal_handler.h"
#include "stream_output.h"

using ::boost::asio::io_context;
using ::boost::core::demangled_name;
using ::boost::format;
using ::std::make_shared;
using ::std::make_unique;
using ::std::raise;
using ::std::shared_ptr;
using ::std::string;
using ::std::vector;

namespace dtee {

int Application::run(int argc, const char* const argv[]) {
	command_line_.parse(argc, argv);

	auto io = make_shared<io_context>();
	auto output = create_dispatch();
	auto input = make_unique<Input>(io, output);
	auto signal_handler = make_unique<SignalHandler>(command_line_, io, output);

	if (!output->open() && !command_line_.cron_mode()) {
		// If we're running from cron then we have output an error message
		// but must continue to execute the requested command.
		return process_->exit_status();
	}

	pid_t pid;

	if (input->open()
			&& signal_handler->open()
			&& fork(*io, pid)) {
		if (pid > 0) {
			signal_handler->start(pid);
			input->fork_parent();
			input->start();
			main_loop(*io, *output);
			signal_handler->stop();

			if (cron_) {
				cron_->report();
			}

			signal_handler.reset();
			process_->exit_by_interrupt();
			return process_->exit_status();
		} else {
			process_->fork_child();
			input->fork_child();
		}
	} else if (!command_line_.cron_mode()) {
		// If we're running from cron then we have output an error message
		// but must continue to execute the requested command.
		return process_->exit_status();
	}

	// Files are opened with O_CLOEXEC so these are unnecessary.
	output.reset();
	cron_.reset();

	// Stop handling signals and close all sockets.
	input.reset();
	signal_handler.reset();

	// This is the only way to really close all file descriptors with Boost.Asio
	// (not everything is CLOEXEC on all platforms).
	io.reset();

	execute(command_line_.command());
	return process_->exit_status();
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

	if (cron_) {
		result_handlers.push_back(cron_);
	}

	return result_handlers;
}

bool Application::fork(boost::asio::io_context &io, pid_t &pid) {
	io.notify_fork(io_context::fork_event::fork_prepare);

	errno = 0;
	pid = ::fork();
	if (pid > 0) {
		io.notify_fork(io_context::fork_event::fork_parent);
		return true;
	} else if (pid == 0) {
		io.notify_fork(io_context::fork_event::fork_child);
		return true;
	} else {
		auto errno_copy = errno;

		io.notify_fork(io_context::fork_event::fork_parent);

		// i18n: %1 = function call name; %2 = errno message
		print_system_error(format(_("%1%: %2%")) % "fork", errno_copy);

		process_->error(ErrorType::FORK);
		return false;
	}
}

void Application::main_loop(boost::asio::io_context &io, ResultHandler &output) {
	try {
		// Wait for events until the I/O service is explicitly stopped
		do {
			io.run();
		} while (!io.stopped());

		// Poll until there are no more events
		do {
			io.reset();
		} while (io.poll() > 0);
	} catch (const std::exception &e) {
		// i18n: %1 = exception type name; %2 = exception message
		print_error(format(_("%1%: %2%")) % demangled_name(typeid(e)), e);

		// Stop immediately because there is no guarantee that further I/O
		// handling will work without either missing SIGCHLD for the command
		// (which would prevent us from ever exiting) or outputting further
		// exception messages indefinitely.
		output.error(ErrorType::MAIN_LOOP_EXCEPTION);
	}
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

	coverage_finish(); // LCOV_EXCL_LINE

	errno = 0;
	execvp(argv[0], &argv.data()[1]);

	auto errno_copy = errno;
	// i18n: %1 = command name; %2 = errno message
	print_system_error(format(_("%1%: %2%")) % argv[0], errno_copy);

	process_->error(ErrorType::EXECUTE_COMMAND);
}

} // namespace dtee
