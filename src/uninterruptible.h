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
#ifndef DTEE_UNINTERRUPTIBLE_H_
#define DTEE_UNINTERRUPTIBLE_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <string>

namespace dtee {

namespace uninterruptible {

int open(const char *pathname, int flags, mode_t mode);
ssize_t read(int fd, char *buf, size_t count);
ssize_t write(int fd, const char *buf, size_t count);
ssize_t write(int fd, const std::string &buf);
off_t lseek(int fd, off_t offset, int whence);
int close(int fd);
pid_t waitpid(pid_t pid, int *wstatus, int options);

} // namespace reliable

} // namespace dtee

#endif
