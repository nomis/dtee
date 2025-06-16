#define lseek64 __dtee__hide__lseek64__
#define lseek __dtee__hide__lseek__

#include <sys/types.h>
#include <errno.h>
#include <unistd.h>

#undef lseek
#undef lseek64

#include "is-dtee.h"
#include "dtee-fcn.h"

static off_t dtee_test_lseek_failure(int fd __attribute__((unused)), off_t offset __attribute__((unused)), int whence __attribute__((unused))) {
	errno = EINVAL;
	return -1;
}

TEST_FCN_REPL(off_t, lseek, (int fd, off_t offset, int whence)) {
	off_t (*next_lseek)(int, off_t, int) = TEST_FCN_NEXT(lseek);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_lseek = dtee_test_lseek_failure;
		}

		active = false;
	}

	return (*next_lseek)(fd, offset, whence);
}

#if defined(__linux__)
static off64_t dtee_test_lseek64_failure(int fd __attribute__((unused)), off64_t offset __attribute__((unused)), int whence __attribute__((unused))) {
	errno = EINVAL;
	return -1;
}

TEST_FCN_REPL(off64_t, lseek64,(int fd, off64_t offset, int whence)) { 
	off64_t (*next_lseek64)(int, off64_t, int) = TEST_FCN_NEXT(lseek64);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_lseek64 = dtee_test_lseek64_failure;
		}

		active = false;
	}

	return (*next_lseek64)(fd, offset, whence);
}
#endif
