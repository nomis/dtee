#include "is-dtee.h"

#include <stdbool.h>
#include <string.h>

static bool is_dtee_cached = false;

static void __attribute__((constructor(1000))) dtee_test_is_dtee_init(void) {
	// Cache this outside of any other function call, otherwise it's too
	// easy to end up with a mess like pthread_init() calling getpid()
	// calling dtee_test_is_dtee() calling open() calling a pthreads
	// function and causing the process to abort().
	is_dtee_cached = __dtee_test_is_dtee_impl();
}

bool dtee_test_is_dtee(void) {
	return is_dtee_cached;
}

bool __dtee_test_is_dtee(const char *exe_name) {
	return !strcmp(exe_name, "dtee") || !strcmp(exe_name, "cronty");
}

bool __dtee_test_is_dtee_test(const char *exe_name) {
	return !strncmp(exe_name, "test-", 5);
}
