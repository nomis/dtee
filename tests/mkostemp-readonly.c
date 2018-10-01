#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "is-dtee.h"

static int dtee_test_mkostemp_readonly(char *template, int flags) {
	// Not a safe implementation of mkostemp but the filename needs to be predictable
	unlink(template);
	flags &= ~(O_WRONLY | O_RDWR);
	flags |= O_RDONLY | O_CREAT | O_EXCL;
	return open(template, flags, S_IRUSR);
}

int mkostemp(char *template, int flags) {
	int (*next_mkostemp)(char *, int) = dlsym(RTLD_NEXT, "mkostemp");

	if (dtee_test_is_dtee()) {
		next_mkostemp = dtee_test_mkostemp_readonly;
	}

	return (*next_mkostemp)(template, flags);
}

int mkostemp64(char *template, int flags) {
	int (*next_mkostemp64)(char *, int) = dlsym(RTLD_NEXT, "mkostemp64");

	if (dtee_test_is_dtee()) {
		next_mkostemp64 = dtee_test_mkostemp_readonly;
	}

	return (*next_mkostemp64)(template, flags);
}
