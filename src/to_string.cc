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
#include "to_string.h"

#include <cstring>
#include <string>
#include <system_error>

#include <boost/format.hpp>

using ::boost::format;
using ::std::string;
using ::std::to_string;

namespace dtee {

string errno_to_string(int errno_copy) {
	return std::error_code(errno_copy, std::system_category()).message();
}

string signal_to_string(int signum) {
	const char *sigdesc = strsignal(signum);

	if (sigdesc != nullptr) {
		return str(format("%1%: %2%") % signum % sigdesc);
	} else {
		return to_string(signum);
	}
}

} // namespace dtee
