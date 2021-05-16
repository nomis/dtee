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
#include "temp_directory.h"

#include <unistd.h>

#include <cstdlib>
#include <cerrno>
#include <exception>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "application.h"
#include "i18n.h"
#include "print_error.h"
#include "temp_filename_pattern.h"

using ::boost::format;
using ::std::string;
using ::std::vector;

namespace dtee {

TempDirectory::TempDirectory(const string &name) {
	try {
		const string pattern = temp_filename_pattern(name);
		vector<char> filename{pattern.cbegin(), pattern.cend() + 1};
		char *temp_dir;

		errno = 0;
		temp_dir = ::mkdtemp(filename.data());
		if (temp_dir == nullptr) {
			auto errno_copy = errno;
			// i18n: %1 = filename; %2 = errno message
			print_system_error(format(_("unable to create temporary directory %1%: %2%")) % pattern, errno_copy);
		} else {
			name_ = temp_dir;
		}
	} catch (const std::exception &e) {
		// i18n: %1 = exception message
		print_error(format(_("unable to create temporary directory: %1%")), e);
	}
}

TempDirectory::~TempDirectory() {
	if (!name_.empty()) {
		for (const auto& filename : files_) {
			unlink(filename.c_str());
		}

		rmdir(name_.c_str());
	}
}

bool TempDirectory::valid() const {
	return !name_.empty();
}

string TempDirectory::register_file(const string &name) {
	string filename = name_ + "/" + name;

	files_.emplace_back(filename);
	return filename;
}

} // namespace dtee
