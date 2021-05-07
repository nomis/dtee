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
#pragma once

#include <cstddef>

#include "result_handler.h"

namespace dtee {

constexpr int SHELL_EXIT_CODE_SIGNAL = 0x80;

class Process: virtual public ResultHandler {
public:
	Process() = default;
	~Process() override = default;

	void terminated(int status, int signum, bool core_dumped) override;
	void interrupted(int signum) override;

	int interrupt_signum();
	int exit_status(int internal_status);

protected:
	bool terminated_ = false; //!< Child process terminated
	int exit_status_ = -1; //!< Exit status of child process
	int exit_signum_ = -1; //!< Termination signal of child process
	bool core_dumped_ = false; //!< Child process produced a core dump
	int interrupt_signum_ = -1; //!< Signal received that caused us to exit
};

} // namespace dtee
