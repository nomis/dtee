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
#include "process.h"

#include <sysexits.h>

#include <csignal>
#include <cstddef>
#include <cstdlib>
#include <vector>

namespace dtee {

using ::std::vector;

void Process::terminated(int status, int signum, bool core_dumped) {
	terminated_ = true;

	exit_status_ = status;
	exit_signum_ = signum;
	core_dumped_ = core_dumped;
}

void Process::interrupted(int signum) {
	interrupt_signum_ = signum;
}

void Process::error(ErrorType type __attribute__((unused))) {
	error_ = true;
}

int Process::interrupt_signum() {
	switch (interrupt_signum_) {
		// Replicate these signals that caused us to terminate.
	case SIGHUP:
	case SIGINT:
	case SIGPIPE:
	case SIGTERM:
		return interrupt_signum_;
	}

	switch (exit_signum_) {
	case SIGINT:
		// Replicate interrupted signal status so that shells
		// behave correctly if the command is interrupted.
		return exit_signum_;
	}

	return -1;
}

int Process::exit_status(int internal_status) {
	int default_status = EX_SOFTWARE;

	// Replicate shell style exit status.
	if (error_) {
		default_status = EX_IOERR;
	} else if (interrupt_signum_ >= 0) {
		// This can only happen if std::raise() failed.
		return SHELL_EXIT_CODE_SIGNAL + interrupt_signum_;
	} else if (exit_status_ == EXIT_SUCCESS) {
		default_status = EXIT_SUCCESS;
	} else if (exit_status_ >= 0) {
		return exit_status_;
	} else if (exit_signum_ >= 0) {
		return SHELL_EXIT_CODE_SIGNAL + exit_signum_;
	}

	if (internal_status != EXIT_SUCCESS) {
		return internal_status;
	} else {
		return default_status;
	}
}

} // namespace dtee
