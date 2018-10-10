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
#ifndef DTEE_TEMP_FILE_H_
#define DTEE_TEMP_FILE_H_

#include <string>

namespace dtee {

class TempFile {
public:
	explicit TempFile(const std::string &name);
	~TempFile();

	bool open();
	std::string name();
	inline int fd() { return fd_; }
	void close();

	TempFile(const TempFile&) = delete;
	TempFile& operator=(const TempFile&) = delete;

private:
	const std::string name_;
	std::string filename_; //!< Temporary output filename (deleted)
	int fd_ = -1; //!< Temporary output file
};

} // namespace dtee

#endif
