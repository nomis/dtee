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
#include "print_error.h"

#include <exception>
#include <iostream>
#include <string>

#include <boost/format.hpp>
#include <boost/system/error_code.hpp>

#include "command_line.h"
#include "i18n.h"

using ::boost::format;
using ::boost::system::error_code;
using ::std::cerr;
using ::std::exception;
using ::std::string;

namespace dtee {

void print_error(const format &message) {
	const string line = str(format(_("%1%: %2%\n")) % CommandLine::display_name() % message);

	cerr.clear();
	cerr.write(line.c_str(), line.length());
}

void print_error(format message, const error_code &ec) {
	print_error(message % ec.message());
}

void print_error(format message, const exception &e) {
	print_error(message % e.what());
}

void print_system_error(format message, string cause) {
	print_error(message % cause);
}

} // namespace dtee
