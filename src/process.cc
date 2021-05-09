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

#ifdef GCOV_ENABLED
extern "C" void __gcov_flush(void);
#endif

using ::std::raise;

namespace dtee {

void Process::terminated(int status, int signum, bool core_dumped) {
	terminated_ = true;

	exit_status_ = status;
	exit_signum_ = signum;
	core_dumped_ = core_dumped;
}

void Process::interrupted(int signum) {
	interrupt_signum_ = signum;
}

void Process::error(ErrorType type) {
	error_ = type;
}

void Process::fork_child() {
	error_ = ErrorType::NONE;
}

void Process::exit_by_interrupt() const {
#ifdef GCOV_ENABLED
	__gcov_flush(); // LCOV_EXCL_LINE
#endif

	switch (interrupt_signum_) {
		// Replicate these signals that caused us to terminate.
	case SIGHUP:
	case SIGINT:
	case SIGPIPE:
	case SIGTERM:
		raise(interrupt_signum_);
		break;
	}

	switch (exit_signum_) {
		// Replicate interrupted signal status so that shells
		// behave correctly if the command is interrupted.
	case SIGINT:
		raise(exit_signum_);
		break;
	}
}

int Process::exit_status() const {
	switch (error_) {
	case ErrorType::OPEN_OUTPUT:
		return EX_CANTCREAT;

	case ErrorType::OPEN_INPUT:
	case ErrorType::SIGNAL_HANDLER:
		return EX_UNAVAILABLE;

	case ErrorType::FORK:
		return EX_OSERR;

	case ErrorType::CLOSE_OUTPUT:
	case ErrorType::READ_INPUT:
	case ErrorType::WRITE_OUTPUT:
		return EX_IOERR;

	case ErrorType::EXECUTE_COMMAND:
		return EX_NOINPUT;

	case ErrorType::MAIN_LOOP_EXCEPTION:
		return EX_SOFTWARE;

	case ErrorType::NONE:
		break;
	}

	// Replicate shell style exit status.
	if (interrupt_signum_ >= 0) {
		// This can only happen if std::raise() failed.
		return SHELL_EXIT_CODE_SIGNAL + interrupt_signum_;
	} else if (exit_status_ >= 0) {
		return exit_status_;
	} else if (exit_signum_ >= 0) {
		return SHELL_EXIT_CODE_SIGNAL + exit_signum_;
	} else {
		return EX_SOFTWARE;
	}
}

} // namespace dtee
