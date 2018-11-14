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
#ifndef DTEE_FILE_OUTPUT_H_
#define DTEE_FILE_OUTPUT_H_

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include <boost/format.hpp>

#include "output.h"
#include "to_string.h"

namespace dtee {

enum class FileOutputType {
	STDOUT,
	STDERR,
	COMBINED,
};

class FileOutput: public Output {
public:
	FileOutput(const std::string &filename, FileOutputType type, bool append);
	~FileOutput() override;

	bool open() override;
	bool output(OutputType type, const std::vector<char> &buffer, size_t len) override;

private:
	void print_file_error(std::string cause = errno_to_string());

	bool filtered_;
	OutputType type_;

	std::string filename_; //!< Output filename
	bool append_;
	int fd_ = -1; //!< Output file
	bool failed_ = false; //!< Failure state of writes to output file
};

} // namespace dtee

#endif
