#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

static int dtee_test_dup2_failure(int oldfd __attribute__((unused)), int newfd __attribute__((unused))) {
	errno = EMFILE;
	return -1;
}

TEST_FCN_REPL(int, dup2, (int oldfd, int newfd)) {
	int (*next_dup2)(int, int) = TEST_FCN_NEXT(dup2);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (newfd == STDOUT_FILENO || newfd == STDERR_FILENO) {
				const char *fail_mode = getenv("DTEE_TEST_DUP2_FAIL");

				if (!fail_mode) {
					abort();
				}

				if (!strcmp(fail_mode, "parent")) {
					if (!dtee_test_is_ppid_dtee()) {
						next_dup2 = dtee_test_dup2_failure;
					}
				} else if (!strcmp(fail_mode, "child")) {
					if (dtee_test_is_ppid_dtee()) {
						next_dup2 = dtee_test_dup2_failure;
					}
				} else {
					abort();
				}
			}
		}

		active = false;
	}

	return (*next_dup2)(oldfd, newfd);
}
