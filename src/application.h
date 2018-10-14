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
#ifndef DTEE_APPLICATION_H_
#define DTEE_APPLICATION_H_

#include <exception>
#include <list>
#include <memory>
#include <string>

#include "command_line.h"
#include "cron.h"
#include "output.h"
#include "file_output.h"
#include "process.h"

namespace dtee {

class Application {
public:
	Application() {};
	~Application() {};

	static void print_error(const std::string &message, const std::string &cause);
	static void print_error(const std::string &message, int errno_copy = 0);
	static void print_error(const std::string &message, const std::exception &e);

	int run(int argc, const char* const argv[]);

	Application(const Application&) = delete;
	Application& operator=(const Application&) = delete;

private:
	void create_file_outputs(std::list<std::shared_ptr<Output>> &outputs,
			const std::string &name, FileOutputType type, bool append);

	std::list<std::shared_ptr<Output>> create_outputs();
	void execute(const std::vector<std::string> &command) __attribute__((noreturn));

	CommandLine command_line_;
	std::shared_ptr<Process> process_;
	std::shared_ptr<Cron> cron_;
};

} // namespace dtee

#endif
