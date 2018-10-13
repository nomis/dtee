#include <sys/types.h>
#include <sys/stat.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "is-dtee.h"

static char *dtee_test_mkdtemp_consistent(char *template) {
	// Not a safe implementation of mkdtemp but the filename needs to be predictable
	rmdir(template);
	if (mkdir(template, S_IRUSR|S_IWUSR|S_IXUSR) < 0) {
		return NULL;
	}
	return template;
}

char *mkdtemp(char *template) {
	char *(*next_mkdtemp)(char *) = dlsym(RTLD_NEXT, "mkdtemp");

	if (dtee_test_is_dtee()) {
		next_mkdtemp = dtee_test_mkdtemp_consistent;
	}

	return (*next_mkdtemp)(template);
}
