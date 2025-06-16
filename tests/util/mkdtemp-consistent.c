#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

static char *dtee_test_mkdtemp_consistent(char *template) {
	// Not a safe implementation of mkdtemp but the filename needs to be predictable
	rmdir(template);
	if (mkdir(template, S_IRUSR|S_IWUSR|S_IXUSR) < 0) {
		return NULL;
	}
	return template;
}

TEST_FCN_REPL(char *, mkdtemp, (char *template)) {
	char *(*next_mkdtemp)(char *) = TEST_FCN_NEXT(mkdtemp);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_mkdtemp = dtee_test_mkdtemp_consistent;
		}

		active = false;
	}

	return (*next_mkdtemp)(template);
}
