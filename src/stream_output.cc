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
#include "stream_output.h"

#include <unistd.h>

#include <cstddef>
#include <cerrno>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "application.h"
#include "i18n.h"
#include "print_error.h"

using ::boost::format;
using ::std::string;
using ::std::vector;

namespace dtee {

StreamOutput::StreamOutput() {
	// i18n: %1 = system call name; %2 = errno message
	stdout_fmt_ = _("stdout: %1%: %2%");
	// i18n: %1 = system call name; %2 = errno message
	stderr_fmt_ = _("stderr: %1%: %2%");
}

bool StreamOutput::open() {
	return true;
}

bool StreamOutput::output(OutputType type, const vector<char> &buffer, size_t len) {
	switch(type) {
	case OutputType::STDOUT:
		return output(STDOUT_FILENO, stdout_fmt_, buffer, len);

	case OutputType::STDERR:
		return output(STDERR_FILENO, stderr_fmt_, buffer, len);
	}

	return false;
}

bool StreamOutput::output(int fd, const char *name_fmt, const vector<char> &buffer, size_t len) {
	// It is not a good idea to write to a pipe ignoring signals because
	// SIGINT can't be used to terminate this process. However, the process
	// on the other end of the pipe will also get the SIGINT so we'll get
	// an EPIPE return value. It would be difficult to make it possible to
	// resume output after handling any received signals.

	errno = 0;
	if (::write(fd, buffer.data(), len) != static_cast<ssize_t>(len)) {
		auto errno_copy = errno;
		print_system_error(format(name_fmt) % "write", errno_copy);
		return false;
	}

	return true;
}

} // namespace dtee
