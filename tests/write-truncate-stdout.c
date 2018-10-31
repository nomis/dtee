#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <unistd.h>

#include "is-dtee.h"

static ssize_t dtee_test_write_truncate(int fd __attribute__((unused)), const void *buf __attribute__((unused)), size_t count __attribute__((unused))) {
	ssize_t (*next_write)(int, const void *, size_t) = dlsym(RTLD_NEXT, "write");

	if (count > 1) {
		count /= 2;
	}

	ssize_t ret = (*next_write)(fd, buf, count);
	if (ret >= 0 && errno == 0) {
		errno = ENOSPC;
	}
	return ret;
}

ssize_t write(int fd, const void *buf, size_t count) {
	ssize_t (*next_write)(int, const void *, size_t) = dlsym(RTLD_NEXT, "write");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (fd == STDOUT_FILENO) {
				next_write = dtee_test_write_truncate;
			}
		}

		active = false;
	}

	return (*next_write)(fd, buf, count);
}