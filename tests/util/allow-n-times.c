#include <stdbool.h>
#include <stdlib.h>

#include "allow-n-times.h"

bool dtee_test_allow_n_times(const char *env_name, bool *first, unsigned long *allowed) {
	const char *allowed_str = getenv(env_name);

	if (*first) {
		if (allowed_str != NULL) {
			*allowed = strtoul(allowed_str, NULL, 10);
		}
		*first = false;
	}

	if (*allowed) {
		--*allowed;
		return true;
	} else {
		return false;
	}
}
