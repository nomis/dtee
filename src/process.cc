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
#include "process.h"

#include <stdlib.h>
#include <vector>

#include "signal.h"

namespace dtee {

using ::std::vector;

bool Process::open() {
	return true;
}

bool Process::output(OutputType type __attribute__((unused)), const vector<char> &buffer __attribute__((unused)), size_t len __attribute__((unused))) {
	return true;
}

void Process::terminated(int status, int signum, bool core_dump __attribute__((unused))) {
	terminated_ = true;

	exit_status_ = status;
	exit_signum_ = signum;
}

int Process::interrupt_signum() {
	switch (exit_signum_) {
		// These could be replicated (the default handler is to
		// terminate) but that behaviour is not strictly accurate,
		// because we have not received them as a signal.
#if 0
	case SIGHUP:
	case SIGKILL:
	case SIGPIPE:
	case SIGALRM:
	case SIGTERM:
#endif

	case SIGINT:
		// Replicate interrupted signal status so that shells
		// behave correctly if the command is interrupted.
		return exit_signum_;
	}

	return -1;
}

int Process::exit_status(int internal_status) {
	int default_status;

	// Replicate shell style exit status.
	if (exit_status_ == EXIT_SUCCESS) {
		default_status = EXIT_SUCCESS;
	} else if (exit_status_ >= 0) {
		return exit_status_;
	} else if (exit_signum_ >= 0){
		return SHELL_EXIT_CODE_SIGNAL + exit_signum_;
	} else {
		default_status = EXIT_FAILURE;
	}

	if (internal_status != EXIT_SUCCESS) {
		return internal_status;
	} else {
		return default_status;
	}
}

} // namespace dtee
