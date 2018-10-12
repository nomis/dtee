#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#include "is-dtee.h"

static char *dtee_test_mkdtemp_failure(char *template __attribute__((unused))) {
	errno = EROFS;
	return NULL;
}

char *mkdtemp(char *template) {
	char *(*next_mkdtemp)(char *) = dlsym(RTLD_NEXT, "mkdtemp");

	if (dtee_test_is_dtee()) {
		next_mkdtemp = dtee_test_mkdtemp_failure;
	}

	return (*next_mkdtemp)(template);
}
