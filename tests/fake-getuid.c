#include <sys/types.h>
#include <dlfcn.h>
#include <unistd.h>

#include "is-dtee.h"

static uid_t dtee_test_fake_getuid(void) {
	return 1;
}

uid_t getuid(void) {
	uid_t (*next_getuid)(void) = dlsym(RTLD_NEXT, "getuid");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_getuid = dtee_test_fake_getuid;
		}

		active = false;
	}

	return (*next_getuid)();
}
