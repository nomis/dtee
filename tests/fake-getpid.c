#include <sys/types.h>
#include <dlfcn.h>
#include <unistd.h>

#include "is-dtee.h"

static pid_t dtee_test_fake_getpid(void) {
	return 1;
}

pid_t getpid(void) {
	pid_t (*next_getpid)(void) = dlsym(RTLD_NEXT, "getpid");

	if (dtee_test_is_dtee()) {
		next_getpid = dtee_test_fake_getpid;
	}

	return (*next_getpid)();
}
