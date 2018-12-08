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
#include "cron.h"

#include <sys/types.h>
#include <unistd.h>
#include <algorithm>
#include <cerrno>
#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <utility>
#include <vector>

#include <boost/format.hpp>

#include "application.h"
#include "print_error.h"
#include "to_string.h"

using ::boost::format;
using ::std::min;
using ::std::shared_ptr;
using ::std::string;
using ::std::vector;

namespace dtee {

Cron::Cron(string command, shared_ptr<Output> fallback)
		: command_(command),
		  fallback_(fallback),
		  file_("output") {

}

void Cron::print_file_error(format message) {
	print_system_error(message % file_.name());
}

bool Cron::open() {
	buffered_ = file_.open();
	return buffered_;
}

bool Cron::output(OutputType type, const vector<char> &buffer, size_t len) {
	bool success = true;

	if (type == OutputType::STDERR) {
		error_ = true;
		success &= unspool_buffer_file();
	}

	if (buffered_) {
		errno = 0;
		ssize_t written = ::write(file_.fd(), buffer.data(), len);
		if (written != static_cast<ssize_t>(len)) {
			print_file_error(format("error writing to buffer file %1%: %2%"));
			error_ = true;

			unspool_buffer_file();

			if (written < 0) {
				written = 0;
			}

			vector<char> buffer_copy{buffer.cbegin() + written, buffer.cend()};
			fallback_->output(type, buffer_copy, len - written);
			success = false;
		}
	} else {
		success &= fallback_->output(type, buffer, len);
	}

	return success;
}

void Cron::terminated(int status, int signum, bool core_dumped) {
	Process::terminated(status, signum, core_dumped);

	if (status != EXIT_SUCCESS) {
		error_ = true;
	}
}

void Cron::interrupted(int signum) {
	Process::interrupted(signum);

	error_ = true;

	// The return status of this is ignored, so an I/O error won't be
	// recorded but if we've been interrupted then the exit status has
	// already been altered.
	unspool_buffer_file();
}

bool Cron::unspool_buffer_file() {
	bool success = true;

	if (buffered_) {
		errno = 0;
		if (::lseek(file_.fd(), 0, SEEK_SET) != 0) {
			print_file_error(format("error seeking to start of buffer file %1%: %2%"));
			success = false;
		} else {
			ssize_t len;
			do {
				// Be compatible with both file and pipe outputs
				vector<char> buf(min(BUFSIZ, PIPE_BUF));

				errno = 0;
				len = ::read(file_.fd(), buf.data(), buf.size());
				if (len < 0) {
					print_file_error(format("error reading buffer file %1%: %2%"));
					success = false;
					break;
				} else if (len > 0) {
					success &= fallback_->output(OutputType::STDOUT, buf, len);
				}
			} while (len > 0);
		}

		file_.close();
		buffered_ = false;
	}

	return success;
}

bool Cron::report() {
	if (terminated_ && !error_) {
		return true;
	}

	bool success = unspool_buffer_file();

	if (interrupt_signum_ >= 0) {
		print_error(format("received signal %1%") % signal_to_string(interrupt_signum_));
	}

	if (exit_status_ >= 0) {
		print_error(format("%1%: exited with status %2%") % command_ % exit_status_);
	} else if (exit_signum_ >= 0) {
		if (core_dumped_) {
			print_error(format("%1%: process terminated by signal %2% (core dumped)") % command_ % signal_to_string(exit_signum_));
		} else {
			print_error(format("%1%: process terminated by signal %2%") % command_ % signal_to_string(exit_signum_));
		}
	}

	return success;
}

} // namespace dtee
