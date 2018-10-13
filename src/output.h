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
#ifndef DTEE_OUTPUT_H_
#define DTEE_OUTPUT_H_

#include <vector>

namespace dtee {

enum class OutputType {
	STDOUT,
	STDERR,
};

class Output {
public:
	Output() {};
	virtual ~Output() {};

	virtual bool open() = 0;
	virtual bool output(OutputType type, const std::vector<char> &buffer, size_t len) = 0;
	virtual void terminated(int status __attribute__((unused)),
			int signum __attribute__((unused)),
			bool core_dumped __attribute__((unused))) {};
	virtual void interrupted(int signum __attribute__((unused))) {};

	Output(const Output&) = delete;
	Output& operator=(const Output&) = delete;
};

} // namespace dtee

#endif
