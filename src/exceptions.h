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
#ifndef DTEE_EXCEPTIONS_H_
#define DTEE_EXCEPTIONS_H_

#include <stdexcept>
#include <string>

namespace dtee {

class FatalError: public std::runtime_error {
public:
	FatalError(int exit_code, const std::string &message, int errno_copy = 0);
	~FatalError();

	int code() const;

	FatalError(const FatalError&) = default;
	FatalError& operator=(const FatalError&) = default;

private:
	int exit_code_;
};

} // namespace dtee

#endif
