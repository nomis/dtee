#define openat64 __dtee__hide__openat64__
#define open64 __dtee__hide__open64__
#define openat __dtee__hide__openat__
#define open __dtee__hide__open__
#define lseek64 __dtee__hide__lseek64__
#define lseek __dtee__hide__lseek__

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
#undef lseek
#undef lseek64

#include "is-dtee.h"

static int dtee_test_open_failure(const char *pathname __attribute__((unused)), int flags __attribute__((unused)), mode_t mode __attribute__((unused))) {
	errno = EINTR;
	return -1;
}

#if defined(__linux__)
static int dtee_test_open64_failure(const char *pathname __attribute__((unused)), int flags __attribute__((unused)), mode_t mode __attribute__((unused))) {
	errno = EINTR;
	return -1;
}
#endif

static int dtee_test_openat_failure(int dirfd __attribute__((unused)), const char *pathname __attribute__((unused)), int flags __attribute__((unused)), mode_t mode __attribute__((unused))) {
	errno = EINTR;
	return -1;
}

#if defined(__linux__)
static int dtee_test_openat64_failure(int dirfd __attribute__((unused)), const char *pathname __attribute__((unused)), int flags __attribute__((unused)), mode_t mode __attribute__((unused))) {
	errno = EINTR;
	return -1;
}
#endif

int open(const char *pathname, int flags, mode_t mode) {
	int (*next_open)(const char *, int, mode_t) = dlsym(RTLD_NEXT, "open");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (!(success = !success)) {
				next_open = dtee_test_open_failure;
			}
		}

		active = false;
	}

	return (*next_open)(pathname, flags, mode);
}

#if defined(__linux__)
int open64(const char *pathname, int flags, mode_t mode) {
	int (*next_open64)(const char *, int, mode_t) = dlsym(RTLD_NEXT, "open64");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (!(success = !success)) {
				next_open64 = dtee_test_open64_failure;
			}
		}

		active = false;
	}

	return (*next_open64)(pathname, flags, mode);
}
#endif

int openat(int dirfd, const char *pathname, int flags, mode_t mode) {
	int (*next_openat)(int, const char *, int, mode_t) = dlsym(RTLD_NEXT, "openat");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (!(success = !success)) {
				next_openat = dtee_test_openat_failure;
			}
		}

		active = false;
	}

	return (*next_openat)(dirfd, pathname, flags, mode);
}

#if defined(__linux__)
int openat64(int dirfd, const char *pathname, int flags, mode_t mode) {
	int (*next_openat64)(int, const char *, int, mode_t) = dlsym(RTLD_NEXT, "openat64");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (!(success = !success)) {
				next_openat64 = dtee_test_openat64_failure;
			}
		}

		active = false;
	}

	return (*next_openat64)(dirfd, pathname, flags, mode);
}
#endif

static ssize_t dtee_test_read_failure(int fd __attribute__((unused)), void *buf __attribute__((unused)), size_t count __attribute__((unused))) {
	errno = EINTR;
	return -1;
}

ssize_t read(int fd, void *buf, size_t count) {
	ssize_t (*next_read)(int, void *, size_t) = dlsym(RTLD_NEXT, "read");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			// Boost.Asio (1.62) communicates signals from the
			// handler to the I/O service using a pipe with
			// reads/writes of the signal number as an int.
			// It does not handle the EINTR error value.
			if (count != sizeof(int)) {
				if (!(success = !success)) {
					next_read = dtee_test_read_failure;
				}
			}
		}

		active = false;
	}

	return (*next_read)(fd, buf, count);
}

static ssize_t dtee_test_write_failure(int fd __attribute__((unused)), const void *buf __attribute__((unused)), size_t count __attribute__((unused))) {
	errno = EINTR;
	return -1;
}

ssize_t write(int fd, const void *buf, size_t count) {
	ssize_t (*next_write)(int, const void *, size_t) = dlsym(RTLD_NEXT, "write");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		// Boost.Asio (1.62) communicates signals from the
		// handler to the I/O service using a pipe with
		// reads/writes of the signal number as an int.
		// It does not handle the EINTR error value.
		// This also implies we're in a signal handler,
		// so don't do anything unsafe.
		if (count != sizeof(int)) {
			if (dtee_test_is_dtee()) {
				if (!(success = !success)) {
					next_write = dtee_test_write_failure;
				}
			}
		}

		active = false;
	}

	return (*next_write)(fd, buf, count);
}

static off_t dtee_test_lseek_failure(int fd __attribute__((unused)), off_t offset __attribute__((unused)), int whence __attribute__((unused))) {
	errno = EINTR;
	return -1;
}

off_t lseek(int fd, off_t offset, int whence) {
	off_t (*next_lseek)(int, off_t, int) = dlsym(RTLD_NEXT, "lseek");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (!(success = !success)) {
				next_lseek = dtee_test_lseek_failure;
			}
		}

		active = false;
	}

	return (*next_lseek)(fd, offset, whence);
}

#if defined(__linux__)
static off64_t dtee_test_lseek64_failure(int fd __attribute__((unused)), off64_t offset __attribute__((unused)), int whence __attribute__((unused))) {
	errno = EINTR;
	return -1;
}

off64_t lseek64(int fd, off64_t offset, int whence) {
	off64_t (*next_lseek64)(int, off64_t, int) = dlsym(RTLD_NEXT, "lseek64");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (!(success = !success)) {
				next_lseek64 = dtee_test_lseek64_failure;
			}
		}

		active = false;
	}

	return (*next_lseek64)(fd, offset, whence);
}
#endif

static int dtee_test_close_failure(int fd __attribute__((unused))) {
#if defined(__linux__) || defined(__FreeBSD__)
	int (*next_close)(int) = dlsym(RTLD_NEXT, "close");
	int ret = (*next_close)(fd);
	if (ret < 0) {
		return ret;
	}
#endif
	errno = EINTR;
	return -1;
}

int close(int fd) {
	int (*next_close)(int) = dlsym(RTLD_NEXT, "close");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (!(success = !success)) {
				next_close = dtee_test_close_failure;
			}
		}

		active = false;
	}

	return (*next_close)(fd);
}

static int dtee_test_dup2_failure(int oldfd __attribute__((unused)), int newfd __attribute__((unused))) {
	errno = EINTR;
	return -1;
}

int dup2(int oldfd, int newfd) {
	int (*next_dup2)(int, int) = dlsym(RTLD_NEXT, "dup2");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (!(success = !success)) {
				next_dup2 = dtee_test_dup2_failure;
			}
		}

		active = false;
	}

	return (*next_dup2)(oldfd, newfd);
}

static pid_t dtee_test_waitpid_failure(pid_t pid __attribute__((unused)), int *wstatus __attribute__((unused)), int options __attribute__((unused))) {
	errno = EINTR;
	return -1;
}

pid_t waitpid(pid_t pid, int *wstatus, int options) {
	pid_t (*next_waitpid)(pid_t, int *, int) = dlsym(RTLD_NEXT, "waitpid");
	static __thread bool active = false;
	static __thread bool success = true;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (!(success = !success)) {
				next_waitpid = dtee_test_waitpid_failure;
			}
		}

		active = false;
	}

	return (*next_waitpid)(pid, wstatus, options);
}
