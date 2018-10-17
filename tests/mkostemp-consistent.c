#define mkostemp64 __dtee__hide__mkostemp64__
#define mkostemp __dtee__hide__mkostemp__

#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#undef mkostemp
#undef mkostemp64

#include "is-dtee.h"

static int dtee_test_mkostemp_consistent(char *template, int flags) {
	// Not a safe implementation of mkostemp but the filename needs to be predictable
	unlink(template);
	flags &= ~(O_RDONLY | O_WRONLY);
	flags |= O_RDWR | O_CREAT | O_EXCL;
	return open(template, flags, S_IRUSR);
}

int mkostemp(char *template, int flags) {
	int (*next_mkostemp)(char *, int) = dlsym(RTLD_NEXT, "mkostemp");

	if (dtee_test_is_dtee()) {
		next_mkostemp = dtee_test_mkostemp_consistent;
	}

	return (*next_mkostemp)(template, flags);
}

int mkostemp64(char *template, int flags) {
	int (*next_mkostemp64)(char *, int) = dlsym(RTLD_NEXT, "mkostemp64");

	if (dtee_test_is_dtee()) {
		next_mkostemp64 = dtee_test_mkostemp_consistent;
	}

	return (*next_mkostemp64)(template, flags);
}
