#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>

#include "is-dtee.h"

static int dtee_test_dup2_failure(int oldfd __attribute__((unused)), int newfd __attribute__((unused))) {
	errno = EMFILE;
	return -1;
}

int dup2(int oldfd, int newfd) {
	int (*next_dup2)(int, int) = dlsym(RTLD_NEXT, "dup2");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee() && dtee_test_is_ppid_dtee()) {
			if (newfd == STDOUT_FILENO || newfd == STDERR_FILENO) {
				next_dup2 = dtee_test_dup2_failure;
			}
		}

		active = false;
	}

	return (*next_dup2)(oldfd, newfd);
}
