#include <dlfcn.h>
#include <errno.h>
#include <unistd.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

static pid_t dtee_test_fork_failure(void) {
	errno = EAGAIN;
	return -1;
}

TEST_FCN_REPL(pid_t, fork, (void)) {
	pid_t (*next_fork)(void) = TEST_FCN_NEXT(fork);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_fork = dtee_test_fork_failure;
		}

		active = false;
	}

	return (*next_fork)();
}
