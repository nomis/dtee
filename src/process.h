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
#ifndef DTEE_PROCESS_H_
#define DTEE_PROCESS_H_

#include <cstddef>

#include "output.h"

namespace dtee {

constexpr int SHELL_EXIT_CODE_SIGNAL = 0x80;

class Process: public Output {
public:
	Process() {};
	virtual ~Process() {};

	bool open() override;
	bool output(OutputType type, const std::vector<char> &buffer, size_t len) override;
	void terminated(int status, int signum, bool core_dump) override;
	void interrupted(int signum) override;

	int interrupt_signum();
	int exit_status(int internal_status);

private:
	bool terminated_ = false; //!< Child process terminated
	int exit_status_ = -1; //!< Exit status of child process
	int exit_signum_ = -1; //!< Termination signal of child process
	int interrupt_signum_ = -1; //!< Signal received that caused us to exit
};

} // namespace dtee

#endif
