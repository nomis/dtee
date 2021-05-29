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

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "output.h"
#include "process.h"
#include "result_handler.h"
#include "temp_file.h"

namespace dtee {

class Cron: public Output, public virtual ResultHandler, private Process {
public:
	Cron(std::string command, std::shared_ptr<Output> fallback);
	~Cron() override = default;

	bool open() override;
	bool output(OutputType type, const std::vector<char> &buffer, size_t len) override;

	void terminated(int status, int signum, bool core_dump) override;
	void interrupted(int signum) override;
	void error(ErrorType type) override;

	bool report();

private:
	void print_file_error(const char *message_fmt, int errno_copy);
	bool unspool_buffer_file();

	std::string command_; //!< Name of command being executed
	std::shared_ptr<Output> fallback_; //!< Fallback output if temporary file can't be created
	TempFile file_; //!< Temporary output file

	bool buffered_ = false; //!< Output is being buffered
	bool report_ = false; //!< Output a status report
};

} // namespace dtee
