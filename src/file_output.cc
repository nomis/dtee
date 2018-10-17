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
#include "file_output.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <sysexits.h>
#include <unistd.h>

#include "application.h"

using ::std::string;

namespace dtee {

FileOutput::FileOutput(const string &filename, FileOutputType type, bool append)
	: filename_(filename),
	  append_(append) {
	switch (type) {
	case FileOutputType::STDOUT:
		filtered_ = true;
		type_ = OutputType::STDOUT;
		break;

	case FileOutputType::STDERR:
		filtered_ = true;
		type_ = OutputType::STDERR;
		break;

	case FileOutputType::COMBINED:
		filtered_ = false;
		break;
	}
}

FileOutput::~FileOutput() {
	if (fd_ >= 0) {
		close(fd_);
	}
}

bool FileOutput::open() {
	// std::fstream can't do O_CLOEXEC
	int flags = O_WRONLY | O_CREAT | O_CLOEXEC | O_NOCTTY;

	if (append_) {
		flags |= O_APPEND;
	} else {
		flags |= O_TRUNC;
	}

	errno = 0;
	fd_ = ::open(filename_.c_str(), flags, DEFFILEMODE);
	if (fd_ < 0) {
		Application::print_error(filename_, errno);

		return false;
	} else {
		return true;
	}
}

bool FileOutput::output(OutputType type, const std::vector<char> &buffer, size_t len) {
	if (!filtered_ || type == type_) {
		if (fd_ >= 0) {
			ssize_t written = write(fd_, buffer.data(), len);
			if (written != static_cast<ssize_t>(len)) {
				if (!failed_) {
					Application::print_error(filename_, errno);

					failed_ = true;
				}
			} else {
				failed_ = false;
			}
		}
	}

	return !failed_;
}

} // namespace dtee
