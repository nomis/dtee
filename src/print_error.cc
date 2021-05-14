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
#include "print_error.h"

#include <exception>
#include <iostream>
#include <string>
#include <system_error>

#include <boost/format.hpp>
#include <boost/system/error_code.hpp>

#include "command_line.h"
#include "i18n.h"

using ::boost::format;
using ::std::cerr;
using ::std::string;

namespace dtee {

void print_error(const format &message) {
	cerr.clear();
	// i18n: %1 = program name; %2 = error message
	cerr << (format(_("%1%: %2%\n")) % CommandLine::display_name() % message).str();
}

void print_error(format message, const boost::system::error_code &ec) {
	print_error(message % ec.message());
}

void print_error(format message, const std::exception &e) {
	print_error(message % e.what());
}

void print_system_error(format message, int errno_copy) {
	print_error(message % std::error_code(errno_copy, std::system_category()).message());
}

} // namespace dtee
