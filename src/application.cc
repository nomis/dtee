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

#include <boost/format.hpp>

#include "command_line.h"
#include "copy.h"
#include "cron.h"
#include "file_output.h"
#include "input.h"
#include "process.h"
#include "stream_output.h"
#include "to_string.h"
#include "uninterruptible.h"

using ::boost::format;
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
	// If we use std::cerr then the write could be interrupted by a signal
	// and the message would be lost.
	uninterruptible::write(STDERR_FILENO, str(format("%s: %s\n") % CommandLine::display_name() % message));
}

int Application::run(int argc, const char* const argv[]) {
	command_line_.parse(argc, argv);

	shared_ptr<Copy> outputs = make_shared<Copy>(create_outputs());
	shared_ptr<Input> input = make_shared<Input>(command_line_, outputs);

	bool outputs_ok = outputs->open();
	bool input_ok = input->open();
	int ret_internal = EXIT_SUCCESS;

	if (!outputs_ok) {
		ret_internal = EX_CANTCREAT;

		if (!command_line_.cron_mode()) {
			// If we're running from cron then we have output an error message
			// for the failed output files but must continue to execute the
			// requested command.
			return ret_internal;
		}
	}

	if (input_ok) {
		input->fork_prepare();
		pid_t pid = fork();
		if (pid > 0) {
			if (!input->fork_parent(pid)) {
				ret_internal = EX_IOERR;
			}

			if (cron_) {
				if (!cron_->report()) {
					ret_internal = EX_IOERR;
				}
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

	outputs.reset(); // Files are opened with O_CLOEXEC so this is unnecessary
	input.reset(); // Stop handling signals and close all sockets

	execute(command_line_.command());
	return EX_SOFTWARE;
}

list<shared_ptr<Output>> Application::create_outputs() {
	list<shared_ptr<Output>> outputs;
	list<shared_ptr<Output>> original;

	process_ = make_shared<Process>();
	outputs.push_back(process_);

	original.push_back(make_shared<StreamOutput>(OutputType::STDOUT));
	original.push_back(make_shared<StreamOutput>(OutputType::STDERR));

	if (command_line_.cron_mode()) {
		cron_ = make_shared<Cron>(
				command_line_.command()[0],
				make_shared<Copy>(original));
		outputs.push_back(cron_);
	} else {
		cron_ = nullptr;
		outputs.splice(outputs.end(), original);
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
