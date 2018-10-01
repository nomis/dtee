#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "is-dtee.h"

static int dtee_test_mkostemp_failure(char *template __attribute__((unused)), int flags __attribute__((unused))) {
	errno = EROFS;
	return -1;
}

int mkostemp(char *template, int flags) {
	int (*next_mkostemp)(char *, int) = dlsym(RTLD_NEXT, "mkostemp");

	if (dtee_test_is_dtee()) {
		next_mkostemp = dtee_test_mkostemp_failure;
	}

	return (*next_mkostemp)(template, flags);
}

int mkostemp64(char *template, int flags) {
	int (*next_mkostemp64)(char *, int) = dlsym(RTLD_NEXT, "mkostemp64");

	if (dtee_test_is_dtee()) {
		next_mkostemp64 = dtee_test_mkostemp_failure;
	}

	return (*next_mkostemp64)(template, flags);
}
