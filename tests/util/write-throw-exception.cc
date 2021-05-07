#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include <stdexcept>

#include "is-dtee.h"
#include "dtee-fcn.h"

extern "C" {

TEST_FCN_DECL(ssize_t, write, (int fd, const void *buf, size_t count));

static ssize_t dtee_test_write_throw_exception(int fd __attribute__((unused)), const void *buf, size_t count) {
	ssize_t (*next_write)(int, const void *, size_t) = reinterpret_cast<ssize_t (*)(int, const void*, size_t)>(TEST_FCN_NEXT(write));

	if (!strncmp(reinterpret_cast<const char*>(buf), "THROW\n", count)) {
		throw std::runtime_error("Hello World!");
	}

	return (*next_write)(fd, buf, count);
}

TEST_FCN_REPL(ssize_t, write, (int fd, const void *buf, size_t count)) {
	ssize_t (*next_write)(int, const void *, size_t) = reinterpret_cast<ssize_t (*)(int, const void*, size_t)>(TEST_FCN_NEXT(write));
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
				next_write = dtee_test_write_throw_exception;
			}
		}

		active = false;
	}

	return (*next_write)(fd, buf, count);
}

}
