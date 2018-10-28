#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>

#include "is-dtee.h"

static off_t dtee_test_lseek_failure(int fd __attribute__((unused)), off_t offset __attribute__((unused)), int whence __attribute__((unused))) {
	errno = EINVAL;
	return -1;
}

off_t lseek(int fd, off_t offset, int whence) {
	off_t (*next_lseek)(int, off_t, int) = dlsym(RTLD_NEXT, "lseek");
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
