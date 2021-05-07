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

#include <cstddef>
#include <string>
#include <vector>

#include "output.h"

namespace dtee {

class StreamOutput: public Output {
public:
	StreamOutput() = default;
	~StreamOutput() override = default;

	bool open() override;
	bool output(OutputType type, const std::vector<char> &buffer, size_t len) override;

private:
	bool output(int fd, const char *name_fmt, const std::vector<char> &buffer, size_t len);
};

} // namespace dtee
