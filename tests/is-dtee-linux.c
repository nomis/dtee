#include "is-dtee.h"

#include <sys/types.h>
#include <libgen.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *dtee_test_read_proc_basename(const char *path, char *buf, size_t buflen) {
	memset(buf, 0, buflen);

	if (readlink(path, buf, buflen - 1) > 0) {
		return basename(buf);
	}

	return "";
}

bool dtee_test_is_dtee(void) {
	bool is_dtee = false;
	static __thread bool active = false;

	if (!active) {
		char buf[PATH_MAX + 1];

		active = true;
		is_dtee = __dtee_test_is_dtee(dtee_test_read_proc_basename("/proc/self/exe", buf, sizeof(buf)));
		active = false;
	}

	return is_dtee;
}

bool dtee_test_is_ppid_dtee(void) {
	bool is_ppid_dtee = false;
	static __thread bool active = false;

	if (!active) {
		char exe[PATH_MAX + 1] = { 0 };

		active = true;
		if (snprintf(exe, sizeof(exe), "/proc/%jd/exe", (intmax_t)getppid()) < (int)sizeof(exe)) {
			char buf[PATH_MAX + 1];
			is_ppid_dtee = __dtee_test_is_dtee(dtee_test_read_proc_basename(exe, buf, sizeof(buf)));
		}
		active = false;
	}

	return is_ppid_dtee;
}

bool dtee_test_is_dtee_test(void) {
	bool is_dtee_test = false;
	static __thread bool active = false;

	if (!active) {
		char buf[PATH_MAX + 1];

		active = true;
		is_dtee_test = __dtee_test_is_dtee_test(dtee_test_read_proc_basename("/proc/self/exe", buf, sizeof(buf)));
		active = false;
	}

	return is_dtee_test;
}
