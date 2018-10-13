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
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <utility>

#include "application.h"

using ::std::cout;
using ::std::cerr;
using ::std::endl;
using ::std::flush;
using ::std::move;
using ::std::shared_ptr;
using ::std::string;
using ::std::to_string;
using ::std::vector;

namespace dtee {

Cron::Cron(string command, shared_ptr<Output> fallback)
		: command_(command),
		  fallback_(fallback),
		  file_("output") {

}

Cron::~Cron() {

}

void Cron::print_file_error(const string &message, int errno_copy) {
	Application::print_error(message + " " + file_.name(), errno_copy);
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
			print_file_error("error writing to buffer file", errno);

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

	if (status >= 0) {
		if (status != EXIT_SUCCESS) {
			error_ = true;
		}
	} else if (signum >= 0) {
		error_ = true;
	} else {
		// The process terminated without an exit status and not by a signal?
		error_ = true;
	}

	exit_status_ = status;
	exit_signum_ = signum;
	core_dumped_ = core_dumped;
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
			print_file_error("error seeking to start of buffer file", errno);
			success = false;
		} else {
			ssize_t len;
			do {
				char buf[PIPE_BUF];

				errno = 0;
				len = read(file_.fd(), buf, sizeof(buf));

				if (len < 0) {
					int errno_copy = errno;

					print_file_error("error reading buffer file", errno_copy);
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

string Cron::signal_to_string(int signum) {
	string message = to_string(signum);
	const char *sigdesc = strsignal(signum);

	if (sigdesc != nullptr) {
		message += ": ";
		message += sigdesc;
	}

	return message;
}

bool Cron::report() {
	if (terminated_ && !error_) {
		return true;
	}

	bool success = unspool_buffer_file();

	if (interrupt_signum_ >= 0) {
		Application::print_error("received signal " + signal_to_string(interrupt_signum_));
	}

	if (exit_status_ >= 0) {
		Application::print_error(command_ + ": exited with status " + to_string(exit_status_));
	} else if (exit_signum_ >= 0) {
		string message = command_ + ": process terminated by signal " + signal_to_string(exit_signum_);

		if (core_dumped_) {
			message += " (core dumped)";
		}

		Application::print_error(message);
	}

	return success;
}

} // namespace dtee
