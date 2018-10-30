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

#include "uninterruptible.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <algorithm>
#include <cerrno>
#include <string>

using std::min;
using std::string;

namespace dtee {

namespace uninterruptible {

int open(const char *pathname, int flags, mode_t mode) {
	int ret;

	do {
		errno = 0;
		ret = ::open(pathname, flags, mode);
	} while (ret < 0 && errno == EINTR);

	return ret;
}

ssize_t read(int fd, char *buf, size_t count) {
	ssize_t ret;

	do {
		errno = 0;
		ret = ::read(fd, static_cast<void*>(buf), count);
	} while (ret < 0 && errno == EINTR);

	return ret;
}

ssize_t write(int fd, const char *buf, size_t count) {
	ssize_t total = 0;
	ssize_t ret;

	do {
		errno = 0;
		ret = ::write(fd, static_cast<const void*>(buf), count);
		if (ret > 0) {
			count -= ret;
			buf += ret;
			total += ret;
		}
	} while ((ret > 0 && count > 0) || (ret < 0 && errno == EINTR));

	if (total > 0) {
		return total;
	} else {
		return ret;
	}
}

ssize_t write(int fd, const string &buf) {
	return write(fd, buf.c_str(), buf.length());
}

off_t lseek(int fd, off_t offset, int whence) {
	off_t ret;

	do {
		errno = 0;
		ret = ::lseek(fd, offset, whence);
	} while (ret < 0 && errno == EINTR);

	return ret;
}

// This program is single-threaded so closing the same file descriptor twice on
// operating systems that guarantee closure even when interrupted will never be
// a problem.
int close(int fd) {
	off_t ret;

	do {
		errno = 0;
		ret = ::close(fd);
#if defined(__linux__) || defined(__FreeBSD__)
		if (errno == EINTR) {
			errno = EINPROGRESS;
		}
#endif
	} while (ret < 0 && errno == EINTR);

	return ret;
}

pid_t waitpid(pid_t pid, int *wstatus, int options) {
	pid_t ret;

	do {
		errno = 0;
		ret = ::waitpid(pid, wstatus, options);
	} while (ret < 0 && errno == EINTR);

	return ret;
}

} // namespace reliable

} // namespace dtee
