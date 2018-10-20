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
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <utility>

#include <boost/format.hpp>

#include "application.h"
#include "to_string.h"

using ::boost::format;
using ::std::cout;
using ::std::cerr;
using ::std::endl;
using ::std::flush;
using ::std::shared_ptr;
using ::std::string;
using ::std::vector;

namespace dtee {

Cron::Cron(string command, shared_ptr<Output> fallback)
		: command_(command),
		  fallback_(fallback),
		  file_("output") {

}

void Cron::print_file_error(format message, string cause) {
	Application::print_error(message % file_.name() % cause);
}

bool Cron::open() {
	buffered_ = file_.open();
	return buffered_;
}

bool Cron::output(OutputType type, const vector<char> &buffer, size_t len) {
	if (type == OutputType::STDERR) {
		error_ = true;
	}

	if (buffered_) {
		errno = 0;
		ssize_t written = write(file_.fd(), buffer.data(), len);
		if (written != static_cast<ssize_t>(len)) {
			print_file_error(format("error writing to buffer file %1%: %2%"));

			unspool_buffer_file();

			if (written < 0) {
				written = 0;
			}

			vector<char> buffer_copy{buffer.cbegin() + written, buffer.cend()};
			fallback_->output(type, buffer_copy, len - written);
			return false;
		}
		return true;
	} else {
		fallback_->output(type, buffer, len);
		return false;
	}
}

void Cron::terminated(int status, int signum, bool core_dumped) {
	terminated_ = true;
	exit_status_ = status;
	exit_signum_ = signum;
	core_dumped_ = core_dumped;

	if (status != EXIT_SUCCESS) {
		error_ = true;
	}
}

void Cron::interrupted(int signum) {
	interrupt_signum_ = signum;
	error_ = true;
}

bool Cron::unspool_buffer_file() {
	bool success = true;

	if (buffered_) {
		errno = 0;
		if (lseek(file_.fd(), 0, SEEK_SET) != 0) {
			print_file_error(format("error seeking to start of buffer file %1%: %2%"));
			success = false;
		} else {
			ssize_t len;
			do {
				char buf[PIPE_BUF];

				errno = 0;
				len = read(file_.fd(), buf, sizeof(buf));

				if (len < 0) {
					print_file_error(format("error reading buffer file %1%: %2%"));
					success = false;
					break;
				}

				cout.write(buf, len);
			} while (len > 0);

			cout << flush;
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
		Application::print_error(format("received signal %1%") % signal_to_string(interrupt_signum_));
	}

	if (exit_status_ >= 0) {
		Application::print_error(format("%1%: exited with status %2%") % command_ % exit_status_);
	} else if (exit_signum_ >= 0) {
		if (core_dumped_) {
			Application::print_error(format("%1%: process terminated by signal %2% (core dumped)") % command_ % signal_to_string(exit_signum_));
		} else {
			Application::print_error(format("%1%: process terminated by signal %2%") % command_ % signal_to_string(exit_signum_));
		}
	}

	return success;
}

} // namespace dtee
