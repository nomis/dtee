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

#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "application.h"
#include "temp_filename_pattern.h"
#include "to_string.h"
#include "uninterruptible.h"

using ::boost::format;
using ::std::string;
using ::std::vector;

namespace dtee {

TempFile::TempFile(const string &name)
		: name_(name) {
}

TempFile::~TempFile() {
	close();
}

bool TempFile::open() {
	const string pattern = temp_filename_pattern(name_);
	vector<char> filename{pattern.cbegin(), pattern.cend() + 1};

	// mkostemp can return EINTR but we don't expect to handle any signals during startup
	errno = 0;
	fd_ = uninterruptible::mkostemp(filename.data(), O_CLOEXEC);
	if (fd_ < 0) {
		Application::print_error(format("unable to create temporary file %1%: %2%") % pattern % errno_to_string());
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
		uninterruptible::close(fd_);
	}
	fd_ = -1;
}

} // namespace dtee
