#include <sys/stat.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "is-dtee.h"

static ssize_t dtee_test_write_truncate(int fd __attribute__((unused)), const void *buf __attribute__((unused)), size_t count __attribute__((unused))) {
	ssize_t (*next_write)(int, const void *, size_t) = dlsym(RTLD_NEXT, "write");

	if (count > 16) {
		count /= 2;
	}

	errno = 0;
	ssize_t ret = (*next_write)(fd, buf, count);
	if (ret >= 0 && errno == 0) {
		errno = EINTR;
	}
	return ret;
}

ssize_t write(int fd, const void *buf, size_t count) {
	ssize_t (*next_write)(int, const void *, size_t) = dlsym(RTLD_NEXT, "write");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			// Boost.Asio (1.62) communicates signals from the
			// handler to the I/O service using a pipe with
			// reads/writes of the signal number as an int.
			// It does not handle the EINTR error value.
			if (count != sizeof(int)) {
				next_write = dtee_test_write_truncate;
			}
		}

		active = false;
	}

	return (*next_write)(fd, buf, count);
}
