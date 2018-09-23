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
#ifndef DTEE_CRONIC_H_
#define DTEE_CRONIC_H_

#include <memory>
#include <string>
#include <vector>

#include "output.h"
#include "temp_file.h"

namespace dtee {

class Cron: public Output {
public:
	explicit Cron(std::string command, std::shared_ptr<Output> fallback);
	virtual ~Cron();
	void output(OutputType type, const std::vector<char> &buffer, size_t len) override;
	void terminated(int status, int signum, bool core_dump) override;
	void report();

	Cron(Cron&&);
	Cron& operator=(Cron&&);

private:
	void print_file_error(const std::string &message, int errno_copy);

	std::string command_; //!< Name of command being executed
	std::shared_ptr<Output> fallback_; //!< Fallback output if temporary file can't be created
	TempFile file_; //!< Temporary output file

	bool buffered_; //!< Output is being buffered
	bool failed_ = false; //!< Failure state of writes to temporary output file

	bool terminated_ = false; //!< Child process terminated
	bool error_ = false; //!< Error state of child process
	int status_ = -1; //!< Exit status of child process
	int signum_ = -1; //!< Termination signal of child process
	bool core_dumped_ = false; //!< Child process produced a core dump
};

} // namespace dtee

#endif
