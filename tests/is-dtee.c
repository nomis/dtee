#include "is-dtee.h"

#include <sys/types.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

static pid_t ppid;

static void __attribute__((constructor)) __dtee_test_getppid_cache(void) {
	// getppid() can change if the parent process exits
	ppid = getppid();
}

pid_t __dtee_test_getppid(void) {
	return ppid;
}

bool __dtee_test_is_dtee(const char *exe_name) {
	return !strcmp(exe_name, "dtee");
}

bool __dtee_test_is_dtee_test(const char *exe_name) {
	return !strncmp(exe_name, "test-", 5);
}
