#include <sys/types.h>
#include <dlfcn.h>
#include <unistd.h>

#include "is-dtee.h"

uid_t getuid(void) {
	if (dtee_test_is_dtee()) {
		return 1;
	} else {
		int (*next_getuid)(void) = dlsym(RTLD_NEXT, "getuid");

		return (*next_getuid)();
	}
}
