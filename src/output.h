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
#pragma once

#include <cstddef>
#include <vector>

namespace dtee {

enum class OutputType {
	STDOUT,
	STDERR,
};

class Output {
public:
	Output() = default;
	virtual ~Output() = default;

	virtual bool open() = 0;
	virtual bool output(OutputType type, const std::vector<char> &buffer, size_t len) = 0;

	Output(const Output&) = delete;
	Output& operator=(const Output&) = delete;
};

} // namespace dtee
