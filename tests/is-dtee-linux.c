#include "is-dtee.h"

#include <sys/types.h>
#include <libgen.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

bool dtee_test_is_dtee(void) {
	bool is_dtee = false;
	char buf[PATH_MAX + 1] = { 0 };
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (readlink("/proc/self/exe", buf, sizeof(buf) - 1) > 0) {
			const char *base_program_name = basename(buf);

			is_dtee = !strcmp(base_program_name, "dtee");
		}

		active = false;
	}

	return is_dtee;
}

bool dtee_test_is_ppid_dtee(void) {
	bool is_ppid_dtee = false;
	char exe[PATH_MAX + 1] = { 0 };
	char buf[PATH_MAX + 1] = { 0 };
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (snprintf(exe, sizeof(exe), "/proc/%jd/exe", (intmax_t)getppid()) < (int)sizeof(exe)) {
			if (readlink(exe, buf, sizeof(buf) - 1) > 0) {
				const char *base_program_name = basename(buf);

				is_ppid_dtee = !strcmp(base_program_name, "dtee");
			}
		}

		active = false;
	}

	return is_ppid_dtee;
}
