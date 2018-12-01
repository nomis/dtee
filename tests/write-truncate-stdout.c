#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

TEST_FCN_DECL(ssize_t, write, (int fd, const void *buf, size_t count));

static ssize_t dtee_test_write_truncate(int fd __attribute__((unused)), const void *buf __attribute__((unused)), size_t count __attribute__((unused))) {
	ssize_t (*next_write)(int, const void *, size_t) = TEST_FCN_NEXT(write);

	if (count > 1) {
		count /= 2;
	}

	ssize_t ret = (*next_write)(fd, buf, count);
	if (ret >= 0 && errno == 0) {
		errno = ENOSPC;
	}
	return ret;
}

TEST_FCN_REPL(ssize_t, write, (int fd, const void *buf, size_t count)) {
	ssize_t (*next_write)(int, const void *, size_t) = TEST_FCN_NEXT(write);
	static __thread bool active = false;

	if (!active) {
		active = true;

		// Boost.Asio (1.62) communicates signals from the
		// handler to the I/O service using a pipe with
		// reads/writes of the signal number as an int.
		// This implies we're in a signal handler,
		// so don't do anything unsafe.
		if (count != sizeof(int)) {
			if (dtee_test_is_dtee()) {
				if (fd == STDOUT_FILENO) {
					next_write = dtee_test_write_truncate;
				}
			}
		}

		active = false;
	}

	return (*next_write)(fd, buf, count);
}
