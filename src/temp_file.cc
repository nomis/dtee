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
#include "temp_file.h"

#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <vector>

#include "application.h"
#include "command_line.h"

using ::std::string;
using ::std::to_string;
using ::std::vector;

namespace dtee {

TempFile::TempFile(const string &name)
		: name_(name) {
}

TempFile::~TempFile() {
	close();
}

bool TempFile::open() {
	string pattern { "/tmp/" + CommandLine::internal_name() +  "." + to_string(getuid()) + "." + to_string(getpid()) + "." + name_ + ".XXXXXX" };
	vector<char> filename {pattern.cbegin(), pattern.cend() + 1};

	errno = 0;
	fd_ = mkostemp(filename.data(), O_CLOEXEC);
	if (fd_ < 0) {
		Application::print_error("unable to create temporary file " + pattern, errno);
		return false;
	} else {
		filename_ = string(filename.data());
		unlink(filename_.c_str());
		return true;
	}
}

string TempFile::name() {
	return filename_;
}

void TempFile::close() {
	if (fd_ >= 0) {
		::close(fd_);
	}
	fd_ = -1;
}

} // namespace dtee
