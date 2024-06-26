#define openat64 __dtee__hide__openat64__
#define open64 __dtee__hide__open64__
#define openat __dtee__hide__openat__
#define open __dtee__hide__open__

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

#undef open
#undef openat
#undef open64
#undef openat64

#include "is-dtee.h"
#include "dtee-fcn.h"

#define DO_N_SUCCESS 2
#define DO_N_FAILURE 3

static int fail_fd = -1;
static int successes = 0;
static int failures = 0;

TEST_FCN_DECL(int, open, (const char *pathname, int flags, mode_t mode));
TEST_FCN_DECL(int, openat, (int dirfd, const char *pathname, int flags, mode_t mode));
#if defined(__linux__)
TEST_FCN_DECL(int, open64, (const char *pathname, int flags, mode_t mode));
TEST_FCN_DECL(int, openat64, (int dirfd, const char *pathname, int flags, mode_t mode));
#endif
TEST_FCN_DECL(ssize_t, write, (int fd, const void *buf, size_t count));
TEST_FCN_DECL(int, close, (int fd));

// When dtee opens the target file, store the fd
static bool dtee_test_match_open(const char *pathname) {
	if (dtee_test_is_dtee() && fail_fd < 0) {
		const char *fail_name = getenv("DTEE_TEST_FILE_FAIL_NAME");

		if (fail_name != NULL && !strcmp(pathname, fail_name)) {
			return true;
		}
	}

	return false;
}

static int dtee_test_return_open(int ret) {
	if (ret >= 0) {
		fail_fd = ret;
	}

	return ret;
}

static int dtee_test_fail_open(const char *pathname __attribute__((unused)),
		int flags __attribute__((unused)), mode_t mode __attribute__((unused))) {
	errno = ENOENT;
	return -1;
}

static int dtee_test_fail_openat(int dirfd __attribute__((unused)),
		const char *pathname __attribute__((unused)),
		int flags __attribute__((unused)), mode_t mode __attribute__((unused))) {
	errno = ENOENT;
	return -1;
}

static int dtee_test_capture_open(const char *pathname, int flags, mode_t mode) {
	int (*next_open)(const char *, int, mode_t) = TEST_FCN_NEXT(open);
	return dtee_test_return_open((*next_open)(pathname, flags, mode));
}

static int dtee_test_capture_openat(int dirfd, const char *pathname, int flags, mode_t mode) {
	int (*next_openat)(int, const char *, int, mode_t) = TEST_FCN_NEXT(openat);
	return dtee_test_return_open((*next_openat)(dirfd, pathname, flags, mode));
}

#if defined(__linux__)
static int dtee_test_capture_open64(const char *pathname, int flags, mode_t mode) {
	int (*next_open64)(const char *, int, mode_t) = TEST_FCN_NEXT(open64);
	return dtee_test_return_open((*next_open64)(pathname, flags, mode));
}

static int dtee_test_capture_openat64(int dirfd, const char *pathname, int flags, mode_t mode) {
	int (*next_openat64)(int, const char *, int, mode_t) = TEST_FCN_NEXT(openat64);
	return dtee_test_return_open((*next_openat64)(dirfd, pathname, flags, mode));
}
#endif

TEST_FCN_REPL(int, open, (const char *pathname, int flags, mode_t mode)) {
	int (*next_open)(const char *, int, mode_t) = TEST_FCN_NEXT(open);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_match_open(pathname)) {
			const char *fail_open = getenv("DTEE_TEST_FILE_OPEN_FAIL");

			if (fail_open && strtoul(fail_open, NULL, 10) == 1) {
				next_open = dtee_test_fail_open;
			} else {
				next_open = dtee_test_capture_open;
			}
		}

		active = false;
	}

	return (*next_open)(pathname, flags, mode);
}

TEST_FCN_REPL(int, openat, (int dirfd, const char *pathname, int flags, mode_t mode)) {
	int (*next_openat)(int, const char *, int, mode_t) = TEST_FCN_NEXT(openat);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_match_open(pathname)) {
			const char *fail_open = getenv("DTEE_TEST_FILE_OPEN_FAIL");

			if (fail_open && strtoul(fail_open, NULL, 10) == 1) {
				next_openat = dtee_test_fail_openat;
			} else {
				next_openat = dtee_test_capture_openat;
			}
		}

		active = false;
	}

	return (*next_openat)(dirfd, pathname, flags, mode);
}

#if defined(__linux__)
TEST_FCN_REPL(int, open64, (const char *pathname, int flags, mode_t mode)) {
	int (*next_open64)(const char *, int, mode_t) = TEST_FCN_NEXT(open64);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_match_open(pathname)) {
			const char *fail_open = getenv("DTEE_TEST_FILE_OPEN_FAIL");

			if (fail_open && strtoul(fail_open, NULL, 10) == 1) {
				next_open64 = dtee_test_fail_open;
			} else {
				next_open64 = dtee_test_capture_open64;
			}
		}

		active = false;
	}

	return (*next_open64)(pathname, flags, mode);
}

TEST_FCN_REPL(int, openat64, (int dirfd, const char *pathname, int flags, mode_t mode)) {
	int (*next_openat64)(int dirfd, const char *, int, mode_t) = TEST_FCN_NEXT(openat64);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_match_open(pathname)) {
			const char *fail_open = getenv("DTEE_TEST_FILE_OPEN_FAIL");

			if (fail_open && strtoul(fail_open, NULL, 10) == 1) {
				next_openat64 = dtee_test_fail_openat;
			} else {
				next_openat64 = dtee_test_capture_openat64;
			}
		}

		active = false;
	}

	return (*next_openat64)(dirfd, pathname, flags, mode);
}
#endif

static int dtee_test_fail_close(int fd) {
	int (*next_close)(int) = TEST_FCN_NEXT(close);
	int ret = (*next_close)(fd);

	if (ret != -1) {
		errno = EIO;
		ret = -1;
	}

	return ret;
}

// If dtee closes the target file, unset the fd
TEST_FCN_REPL(int, close, (int fd)) {
	int (*next_close)(int) = TEST_FCN_NEXT(close);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (fail_fd == fd) {
				const char *fail_close = getenv("DTEE_TEST_FILE_CLOSE_FAIL");

				if (fail_close && strtoul(fail_close, NULL, 10) == 1) {
					next_close = dtee_test_fail_close;
				}

				fail_fd = -1;
			}
		}

		active = false;
	}

	return (*next_close)(fd);
}

static bool dtee_test_allow_write(void) {
	const char *fail_mode = getenv("DTEE_TEST_FILE_WRITE_FAIL");

	if (!fail_mode) {
		return true;
	}

	if (!strcmp(fail_mode, "all")) {
		return false;
	} else if (!strcmp(fail_mode, "intermittent")) {
		// Cause intermittent write failures to the file
		if (successes >= DO_N_SUCCESS) {
			++failures;

			if (failures >= DO_N_FAILURE) {
				successes = 0;
				failures = 0;
			}

			return false;
		} else {
			++successes;

			return true;
		}
	} else {
		return true;
	}
}

static ssize_t dtee_test_write_failure(int fd __attribute__((unused)), const void *buf __attribute__((unused)), size_t count __attribute__((unused))) {
	const char *fail_mode = getenv("DTEE_TEST_FILE_WRITE_FAIL");

	if (!fail_mode) {
		abort();
	}

	if (!strcmp(fail_mode, "all")) {
		errno = EINVAL;
	} else if (!strcmp(fail_mode, "intermittent")) {
		errno = ENOSPC;
	} else {
		abort();
	}

	return -1;
}

TEST_FCN_REPL(ssize_t, write, (int fd, const void *buf, size_t count)) {
	ssize_t (*next_write)(int, const void *, size_t) = TEST_FCN_NEXT(write);
	static __thread bool active = false;

	if (!active) {
		active = true;

		// Boost.Asio (1.62) communicates signals from the
		// handler to the I/O service using a pipe with
		// reads/writes of the signal number as an int.
		//
		// It may also use eventfd (8 bytes).
		//
		// This implies we're in a signal handler, so don't
		// do anything unsafe.
		if (count != sizeof(int) || count == 8) {
			if (dtee_test_is_dtee()) {
				if (fail_fd >= 0 && fd == fail_fd) {
					if (!dtee_test_allow_write()) {
						next_write = dtee_test_write_failure;
					}
				}
			}
		}

		active = false;
	}

	return (*next_write)(fd, buf, count);
}
