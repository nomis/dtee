#include <sys/types.h>
#include <dlfcn.h>
#include <unistd.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

static pid_t dtee_test_fake_getpid(void) {
	return 0xB456;
}

TEST_FCN_REPL(pid_t, getpid, (void)) {
	pid_t (*next_getpid)(void) = TEST_FCN_NEXT(getpid);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_getpid = dtee_test_fake_getpid;
		}

		active = false;
	}

	return (*next_getpid)();
}
