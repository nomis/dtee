#define mkostemp64 __dtee__hide__mkostemp64__
#define mkostemp __dtee__hide__mkostemp__

#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#undef mkostemp
#undef mkostemp64

#include "is-dtee.h"
#include "dtee-fcn.h"

static int cron_fd = -1;

TEST_FCN_DECL(int, mkostemp, (char *template, int flags));
TEST_FCN_DECL(int, mkostemp64, (char *template, int flags));

static int dtee_test_mkostemp_copy(char *template, int flags) {
	int (*next_mkostemp)(char *, int) = TEST_FCN_NEXT(mkostemp);
	int fd = (*next_mkostemp)(template, flags);
	cron_fd = fd;
	return fd;
}

#if defined(__linux__)
static int dtee_test_mkostemp64_copy(char *template, int flags) {
	int (*next_mkostemp64)(char *, int) = TEST_FCN_NEXT(mkostemp64);
	int fd = (*next_mkostemp64)(template, flags);
	cron_fd = fd;
	return fd;
}
#endif

TEST_FCN_REPL(int, mkostemp, (char *template, int flags)) {
	int (*next_mkostemp)(char *, int) = TEST_FCN_NEXT(mkostemp);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_mkostemp = dtee_test_mkostemp_copy;
		}

		active = false;
	}

	return (*next_mkostemp)(template, flags);
}

#if defined(__linux__)
TEST_FCN_REPL(int, mkostemp64, (char *template, int flags)) {
	int (*next_mkostemp64)(char *, int) = TEST_FCN_NEXT(mkostemp64);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_mkostemp64 = dtee_test_mkostemp64_copy;
		}

		active = false;
	}

	return (*next_mkostemp64)(template, flags);
}
#endif

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
				if (fd == cron_fd) {
					next_write = dtee_test_write_truncate;
				}
			}
		}

		active = false;
	}

	return (*next_write)(fd, buf, count);
}
