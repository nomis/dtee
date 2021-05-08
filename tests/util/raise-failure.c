#include <dlfcn.h>
#include <errno.h>
#include <signal.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

static int dtee_test_raise_failure(int sig __attribute__((unused))) {
	errno = EPERM;
	return -1;
}

TEST_FCN_REPL(int, raise, (int sig)) {
	int (*next_raise)(int) = TEST_FCN_NEXT(raise);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_raise = dtee_test_raise_failure;
		}

		active = false;
	}

	return (*next_raise)(sig);
}
