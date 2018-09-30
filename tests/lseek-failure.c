#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>

#include "is-dtee.h"

off_t lseek(int fd, off_t offset, int whence) {
	if (dtee_test_is_dtee()) {
		errno = EINVAL;
		return -1;
	} else {
		off_t (*next_lseek)(int, off_t, int) = dlsym(RTLD_NEXT, "lseek");

		return (*next_lseek)(fd, offset, whence);
	}
}
