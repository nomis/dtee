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
#include "temp_filename_pattern.h"

#include <sys/types.h>
#include <unistd.h>

#if __cplusplus >= 201703L
# include <filesystem>
#endif
#include <string>

#if __cplusplus < 201703L
# include <boost/filesystem.hpp>
#endif
#include <boost/format.hpp>

#include "command_line.h"

#if __cplusplus < 201703L
using ::boost::filesystem::temp_directory_path;
#endif
using ::boost::format;
#if __cplusplus >= 201703L
using ::std::filesystem::temp_directory_path;
#endif
using ::std::string;

namespace dtee {

// The pattern used for temporary filenames must comply with the requirements of both mkdtemp and mkostemp.
//   mkdtemp: The last six characters of template must be "XXXXXX" and these are replaced with a string that makes the directory name unique.
//   mkostemp: The last six characters of template must be "XXXXXX" and these are replaced with a string that makes the filename unique.
string temp_filename_pattern(const string &name) {
	const string pattern_file{str(format("%s%08X%sXXXXXX") % CommandLine::DEFAULT_PROGRAM_NAME % getpid() % name)};
	return (temp_directory_path() / pattern_file).string();
}

} // namespace dtee
