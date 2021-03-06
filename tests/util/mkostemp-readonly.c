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
#include "dtee-fcn.h"

static int dtee_test_mkostemp_readonly(char *template, int flags) {
	// Not a safe implementation of mkostemp but the filename needs to be predictable
	unlink(template);
	flags &= ~(O_WRONLY | O_RDWR);
	flags |= O_RDONLY | O_CREAT | O_EXCL;
	return open(template, flags, S_IRUSR);
}

TEST_FCN_REPL(int, mkostemp, (char *template, int flags)) {
	int (*next_mkostemp)(char *, int) = TEST_FCN_NEXT(mkostemp);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_mkostemp = dtee_test_mkostemp_readonly;
		}

		active = false;
	}

	return (*next_mkostemp)(template, flags);
}

#if defined(__linux__)
TEST_FCN_REPL(int, mkostemp64, (char *template, int flags)) {
	int (*next_mkostemp64)(char *, int) = TEST_FCN_NEXT(mkostemp64);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_mkostemp64 = dtee_test_mkostemp_readonly;
		}

		active = false;
	}

	return (*next_mkostemp64)(template, flags);
}
#endif
