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

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

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

using ::boost::program_options::variables_map;

extern char **environ;

namespace dtee {

Application::Application() {

}

Application::~Application() {

}

string Application::name() {
	return internal_name_;
}

void Application::print_error(const string &message, int errno_copy) {
	cout << flush;

	cerr << display_name_ << ": " << message;
	if (errno_copy != 0) {
		cerr << ": " << std::error_code(errno_copy, std::system_category()).message();
	}
	cerr << endl;
}

int Application::run(int argc, const char* const argv[]) {
	if (argc > 0) {
		update_name(argv[0]);
	}

	variables_map variables;
	parse_command_line(argc, argv, variables);
	cron_mode_ |= variables["cron"].as<bool>();

	Input input{make_shared<Copy>(create_outputs(variables))}; // FIXME need to catch exceptions

	input.fork_prepare();
	pid_t pid = fork();
	if (pid > 0) {
		int ret = input.fork_parent(pid);

		if (cron_) {
			cron_->report();
		}

		return ret;
	} else {
		if (pid == 0) {
			input.fork_child();
		}

		execute(variables[BOOST_COMMAND_OPT].as<std::vector<std::string>>());
		return EXIT_FAILURE;
	}
}

list<shared_ptr<Output>> Application::create_outputs(const variables_map &variables) {
	list<shared_ptr<Output>> outputs;
	list<shared_ptr<Output>> original;

	original.push_back(make_shared<StreamOutput>(cout, OutputType::STDOUT));
	original.push_back(make_shared<StreamOutput>(cerr, OutputType::STDERR));

	if (cron_mode_) {
		cron_ = make_shared<Cron>(
				variables[BOOST_COMMAND_OPT].as<std::vector<std::string>>()[0],
				make_shared<Copy>(original));
		outputs.push_back(cron_);
	}

	create_file_outputs(outputs, variables, "out-overwrite", FileOutputType::STDOUT, false);
	create_file_outputs(outputs, variables, "err-overwrite", FileOutputType::STDERR, false);
	create_file_outputs(outputs, variables, "combined-overwrite", FileOutputType::COMBINED, false);
	create_file_outputs(outputs, variables, "out-append", FileOutputType::STDOUT, true);
	create_file_outputs(outputs, variables, "err-append", FileOutputType::STDERR, true);
	create_file_outputs(outputs, variables, "combined-append", FileOutputType::COMBINED, true);

	if (!cron_mode_) {
		// Put the original outputs last so that what you see has already been written to files
		outputs.splice(outputs.end(), original);
	}

	return outputs;
}

void Application::create_file_outputs(list<shared_ptr<Output>> &outputs,
		const variables_map &variables,
		const string &name, FileOutputType type, bool append) {
	if (variables.count(name)) {
		for (const auto& filename : variables[name].as<vector<string>>()) {
			outputs.push_back(make_shared<FileOutput>(filename, type, append));
		}
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
	exit(EXIT_FAILURE);
}

} // namespace dtee
