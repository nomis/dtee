#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>

#include "is-dtee.h"

static pid_t dtee_test_fork_failure(void) {
	errno = EAGAIN;
	return -1;
}

pid_t fork(void) {
	pid_t (*next_fork)(void) = dlsym(RTLD_NEXT, "fork");

	if (dtee_test_is_dtee()) {
		next_fork = dtee_test_fork_failure;
	}

	return (*next_fork)();
}
