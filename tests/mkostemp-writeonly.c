// Don't include <stdlib.h> because libc6 will define an inline mkostemp() redirect to mkostemp64()
#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <unistd.h>

#include "is-dtee.h"

static int dtee_test_mkostemp_writeonly(char *template, int flags) {
	// Not a safe implementation of mkostemp but the filename needs to be predictable
	unlink(template);
	flags &= ~(O_RDONLY | O_RDWR);
	flags |= O_WRONLY | O_CREAT | O_EXCL;
	return open(template, flags, S_IWUSR);
}

int mkostemp(char *template, int flags) {
	int (*next_mkostemp)(char *, int) = dlsym(RTLD_NEXT, "mkostemp");

	if (dtee_test_is_dtee()) {
		next_mkostemp = dtee_test_mkostemp_writeonly;
	}

	return (*next_mkostemp)(template, flags);
}

int mkostemp64(char *template, int flags) {
	int (*next_mkostemp64)(char *, int) = dlsym(RTLD_NEXT, "mkostemp64");

	if (dtee_test_is_dtee()) {
		next_mkostemp64 = dtee_test_mkostemp_writeonly;
	}

	return (*next_mkostemp64)(template, flags);
}
