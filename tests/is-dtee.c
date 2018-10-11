#include "is-dtee.h"

#include <libgen.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

bool dtee_test_is_dtee(void) {
	bool is_dtee = false;
	char buf[PATH_MAX + 1] = { 0 };

	if (readlink("/proc/self/exe", buf, sizeof(buf) - 1) > 0) {
		const char *base_program_name = basename(buf);

		is_dtee = !strcmp(base_program_name, "dtee");
	}

	return is_dtee;
}
