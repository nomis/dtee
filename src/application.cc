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
#include <errno.h>
#include <signal.h>
#include <sysexits.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

#include "command_line.h"
#include "copy.h"
#include "cron.h"
#include "file_output.h"
#include "input.h"
#include "stream_output.h"

using ::std::list;
using ::std::make_shared;
using ::std::shared_ptr;
using ::std::string;
using ::std::vector;

using ::std::cout;
using ::std::cerr;
using ::std::endl;
using ::std::flush;

extern char **environ;

namespace dtee {

CommandLine Application::command_line_;

void Application::print_error(const string &message, int errno_copy) {
	cout << flush;

	cerr << command_line_.display_name() << ": " << message;
	if (errno_copy != 0) {
		cerr << ": " << std::error_code(errno_copy, std::system_category()).message();
	}
	cerr << endl;
}

int Application::run(int argc, const char* const argv[]) {
	command_line_.parse(argc, argv);

	shared_ptr<Copy> outputs = make_shared<Copy>(create_outputs());
	shared_ptr<Input> input = make_shared<Input>(outputs);

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
			input->fork_parent(pid);

			if (cron_) {
				if (!cron_->report()) {
					ret_internal = EX_IOERR;
				}
			}

			int signum = input->interrupt_signum();

			if (signum >= 0) {
				errno = 0;
				if (signal(signum, SIG_DFL) == SIG_ERR) {
					print_error("signal", errno);
				} else {
					kill(getpid(), signum);
				}
			}

			int ret_child = input->exit_status();

			if (ret_child == EXIT_SUCCESS && ret_internal != EXIT_SUCCESS) {
				return ret_internal;
			} else {
				return ret_child;
			}
		} else {
			if (pid == 0) {
				input->fork_child();
			}

			execute(command_line_.command());
			return EX_SOFTWARE;
		}
	} else if (command_line_.cron_mode()) {
		// If we're running from cron then we have output an error message
		// for the failed preparation to handle input but must continue to
		// execute the requested command. Close open files/sockets first.
		outputs.reset(); // Files are opened with O_CLOEXEC so this is unnecessary
		input.reset(); // Boost (1.62) has no support for SOCK_CLOEXEC

		execute(command_line_.command());
		return EX_SOFTWARE;
	} else {
		return EX_UNAVAILABLE;
	}
}

list<shared_ptr<Output>> Application::create_outputs() {
	list<shared_ptr<Output>> outputs;
	list<shared_ptr<Output>> original;

	original.push_back(make_shared<StreamOutput>(cout, OutputType::STDOUT));
	original.push_back(make_shared<StreamOutput>(cerr, OutputType::STDERR));

	if (command_line_.cron_mode()) {
		cron_ = make_shared<Cron>(
				command_line_.command()[0],
				make_shared<Copy>(original));
		outputs.push_back(cron_);
	} else {
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

	errno = 0;
	execvpe(argv[0], &argv.data()[1], environ);
	print_error(argv[0], errno);
	exit(EX_NOINPUT);
}

} // namespace dtee
