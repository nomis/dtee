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
#pragma once

#include <exception>
#include <string>

#include <boost/format.hpp>
#include <boost/system/error_code.hpp>

namespace dtee {

void print_error(const boost::format &message);

void print_error(boost::format &message, const boost::system::error_code &ec);
void inline print_error(boost::format &&message, const boost::system::error_code &ec) {
	print_error(message, ec);
}

void print_error(boost::format &message, const std::exception &e);
void inline print_error(boost::format &&message, const std::exception &e) {
	print_error(message, e);
}

void print_system_error(boost::format &message, int errno_copy);
void inline print_system_error(boost::format &&message, int errno_copy) {
	print_system_error(message, errno_copy);
}

} // namespace dtee
