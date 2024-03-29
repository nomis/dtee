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
#pragma once

#include <string>
#include <vector>

#include <boost/program_options.hpp>

namespace dtee {

class CommandLine {
public:
	CommandLine() = default;
	~CommandLine() = default;

	static std::string internal_name() { return internal_name_; }
	static std::string display_name() { return display_name_; }

	void parse(int argc, const char* const argv[]);

	bool flag(const std::string &name) const;
	const std::vector<std::string>& list(const std::string &name) const;

	bool cron_mode() const { return cron_mode_; }
	bool ignore_interrupts() const { return flag("ignore-interrupts"); }
	const std::vector<std::string>& command() const { return list(BOOST_COMMAND_OPT); }

	CommandLine(const CommandLine&) = delete;
	CommandLine& operator=(const CommandLine&) = delete;

	static const std::string DEFAULT_PROGRAM_NAME;

private:
	static std::vector<boost::program_options::option> end_of_opts_parser(std::vector<std::string> &args);

	void update_name(const std::string &program_name);
	void display_usage(const boost::program_options::options_description &options) const;
	void display_version() const;
	void display_variables() const;

	static const std::string CRON_MODE_NAME;
	static const std::string BOOST_COMMAND_OPT;
	static std::string internal_name_;
	static std::string display_name_;

	boost::program_options::variables_map variables_;
	bool cron_mode_ = false;
};

} // namespace dtee
