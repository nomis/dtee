#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>

#include "is-dtee.h"

off64_t lseek64(int fd, off64_t offset, int whence) {
	if (dtee_test_is_dtee()) {
		errno = EINVAL;
		return -1;
	} else {
		off_t (*next_lseek64)(int, off64_t, int) = dlsym(RTLD_NEXT, "lseek64");

		return (*next_lseek64)(fd, offset, whence);
	}
}
