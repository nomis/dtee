#include <sys/types.h>
#include <dlfcn.h>
#include <unistd.h>

#include "is-dtee.h"

pid_t getpid(void) {
	if (dtee_test_is_dtee()) {
		return 1;
	} else {
		int (*next_getpid)(void) = dlsym(RTLD_NEXT, "getpid");

		return (*next_getpid)();
	}
}