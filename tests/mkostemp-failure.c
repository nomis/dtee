#define mkostemp64 __dtee__hide__mkostemp64__
#define mkostemp __dtee__hide__mkostemp__

#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>

#undef mkostemp
#undef mkostemp64

#include "is-dtee.h"
#include "dtee-fcn.h"

static int dtee_test_mkostemp_failure(char *template __attribute__((unused)), int flags __attribute__((unused))) {
	errno = EROFS;
	return -1;
}

TEST_FCN_REPL(int, mkostemp, (char *template, int flags)) {
	int (*next_mkostemp)(char *, int) = TEST_FCN_NEXT(mkostemp);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_mkostemp = dtee_test_mkostemp_failure;
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
			next_mkostemp64 = dtee_test_mkostemp_failure;
		}

		active = false;
	}

	return (*next_mkostemp64)(template, flags);
}
#endif
