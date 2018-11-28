#include "is-dtee.h"

#include <sys/types.h>
#include <sys/sysctl.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

static const char *dtee_test_read_argv0_basename(pid_t pid, char *buf, size_t buflen) {
	int mib[] = { CTL_KERN, KERN_PROCARGS2, pid };

	memset(buf, 0, buflen);

	size_t proclen = 0;
	if (sysctl(mib, ARRAY_SIZE(mib), NULL, &proclen, NULL, 0) == 0) {
		proclen += BUFSIZ;
		char procbuf[proclen];

		if (sysctl(mib, ARRAY_SIZE(mib), procbuf, &proclen, NULL, 0) == 0) {
			strncpy(buf, procbuf + sizeof(int), buflen - 1);
			return basename(buf);
		}
	}

	return "";
}

bool __dtee_test_is_dtee_impl(void) {
	bool is_dtee = false;
	static __thread bool active = false;

	if (!active) {
		char buf[PATH_MAX + 1];

		active = true;
		is_dtee = __dtee_test_is_dtee(dtee_test_read_argv0_basename(getpid(), buf, sizeof(buf)));
		active = false;
	}

	return is_dtee;
}

bool dtee_test_is_ppid_dtee(void) {
	bool is_ppid_dtee = false;
	static __thread bool active = false;

	if (!active) {
		char buf[PATH_MAX + 1];

		active = true;
		is_ppid_dtee = __dtee_test_is_dtee(dtee_test_read_argv0_basename(getppid(), buf, sizeof(buf)));
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
		is_dtee_test = __dtee_test_is_dtee_test(dtee_test_read_argv0_basename(getpid(), buf, sizeof(buf)));
		active = false;
	}

	return is_dtee_test;
}
