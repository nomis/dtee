#include "is-dtee.h"

#include <stdbool.h>
#include <string.h>

bool __dtee_test_is_dtee(const char *exe_name) {
	return !strcmp(exe_name, "dtee");
}

bool __dtee_test_is_dtee_test(const char *exe_name) {
	return !strncmp(exe_name, "test-", 5);
}
