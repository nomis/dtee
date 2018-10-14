#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>

#include "is-dtee.h"

static off64_t dtee_test_lseek64_failure(int fd __attribute__((unused)), off64_t offset __attribute__((unused)), int whence __attribute__((unused))) {
	errno = EINVAL;
	return -1;
}

off64_t lseek64(int fd, off64_t offset, int whence) {
	off_t (*next_lseek64)(int, off64_t, int) = dlsym(RTLD_NEXT, "lseek64");

	if (dtee_test_is_dtee()) {
		next_lseek64 = dtee_test_lseek64_failure;
	}

	return (*next_lseek64)(fd, offset, whence);
}
