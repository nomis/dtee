#include <errno.h>
#include <stdlib.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

static char *dtee_test_mkdtemp_failure(char *template __attribute__((unused))) {
	errno = EROFS;
	return NULL;
}

TEST_FCN_REPL(char *, mkdtemp, (char *template)) {
	char *(*next_mkdtemp)(char *) = TEST_FCN_NEXT(mkdtemp);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_mkdtemp = dtee_test_mkdtemp_failure;
		}

		active = false;
	}

	return (*next_mkdtemp)(template);
}
