#include <sys/types.h>
#include <sys/wait.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

static pid_t dtee_test_waitpid_failure(pid_t pid __attribute__((unused)), int *wstatus __attribute__((unused)), int options __attribute__((unused))) {
	const char *type = getenv("DTEE_TEST_WAITPID_FAILURE_TYPE");

	if (!strcmp(type, "NO_CHILDREN")) {
		errno = ECHILD;
		return -1;
	} else if (!strcmp(type, "NO_CHANGE")) {
		errno = 0;
		return 0;
	} else {
		abort();
	}
}

TEST_FCN_REPL(pid_t, waitpid, (pid_t pid, int *wstatus, int options)) {
	pid_t (*next_waitpid)(pid_t, int *, int) = TEST_FCN_NEXT(waitpid);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_waitpid = dtee_test_waitpid_failure;
		}

		active = false;
	}

	return (*next_waitpid)(pid, wstatus, options);
}
