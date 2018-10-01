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
#include "exceptions.h"

#include <stdexcept>
#include <string>

#include "application.h"

using ::std::string;

namespace dtee {

FatalError::FatalError(int exit_code, const string &message, int errno_copy)
		: std::runtime_error(errno_copy == 0 ? message : (message + ": " + std::error_code(errno_copy, std::system_category()).message())),
		  exit_code_(exit_code) {

}

FatalError::~FatalError() {

}

int FatalError::code() const {
	return exit_code_;
}

} // namespace dtee
