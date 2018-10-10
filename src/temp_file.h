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
	TempFile();
	explicit TempFile(const std::string &name);
	~TempFile();

	std::string name();
	bool valid();
	void close();
	inline int fd() { return fd_; }

	TempFile(TempFile&& rhs);
	TempFile& operator=(TempFile&& rhs);

private:
	std::string name_; //!< Temporary output filename (deleted)
	int fd_ = -1; //!< Temporary output file
};

} // namespace dtee

#endif
