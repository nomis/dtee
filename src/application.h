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

#include <list>
#include <memory>
#include <string>

#include <boost/program_options.hpp>

#include "cron.h"
#include "output.h"
#include "file_output.h"

namespace dtee {

constexpr int SHELL_EXIT_CODE_SIGNAL = 0x80;

class Application {
public:
	Application();
	~Application();

	static std::string name();
	static void print_error(const std::string &message, int errno_copy = 0);

	int run(int argc, const char* const argv[]);

private:
	static std::vector<boost::program_options::option> end_of_opts_parser(std::vector<std::string> &args);
	static void create_file_outputs(std::list<std::shared_ptr<Output>> &outputs,
			const boost::program_options::variables_map &variables,
			const std::string &name, FileOutputType type, bool append);

	void update_name(const std::string &program_name);
	void parse_command_line(int argc, const char* const argv[], boost::program_options::variables_map &variables) const;
	void display_usage(boost::program_options::options_description &options) const;
	void display_version() const;
	void display_variables(const boost::program_options::variables_map &variables) const;
	std::list<std::shared_ptr<Output>> create_outputs(const boost::program_options::variables_map &variables);
	void execute(const std::vector<std::string> &command) __attribute__((noreturn));

	static const std::string DEFAULT_PROGRAM_NAME;
	static const std::string CRON_MODE_NAME;
	static const std::string BOOST_COMMAND_OPT;

	static std::string internal_name_;
	static std::string display_name_;

	bool cron_mode_ = false;
	std::shared_ptr<Cron> cron_;
};

} // namespace dtee

#endif
