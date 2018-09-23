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
#include "temp_directory.h"

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <vector>

#include "application.h"

using ::std::string;
using ::std::to_string;
using ::std::vector;

namespace dtee {

TempDirectory::TempDirectory(const string &name) {
	string pattern { "/tmp/" + Application::name() +  "." + to_string(getuid()) + "." + to_string(getpid()) + "." + name + ".XXXXXX" };
	vector<char> filename {pattern.cbegin(), pattern.cend() + 1};
	char *temp_dir;

	errno = 0;
	temp_dir = mkdtemp(filename.data());
	if (temp_dir == nullptr) {
		Application::print_error("unable to create temporary directory " + pattern, errno);
	} else {
		name_ = temp_dir;
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

bool TempDirectory::valid() {
	return !name_.empty();
}

string TempDirectory::register_file(const string &name) {
	string filename = name_ + "/" + name;

	files_.emplace_back(filename);
	return filename;
}

} // namespace dtee
