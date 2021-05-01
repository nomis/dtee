#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <signal.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

static int dtee_test_kill_failure(pid_t pid __attribute__((unused)), int sig __attribute__((unused))) {
	errno = EPERM;
	return -1;
}

TEST_FCN_REPL(int, kill, (pid_t pid, int sig)) {
	int (*next_kill)(pid_t, int) = TEST_FCN_NEXT(kill);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_kill = dtee_test_kill_failure;
		}

		active = false;
	}

	return (*next_kill)(pid, sig);
}
