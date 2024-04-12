#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

TEST_FCN_DECL(int, fcntl, (int fd, int cmd, ...));

static bool dtee_test_fcntl_match_failure(int fd, int cmd) {
	if (cmd == F_GETFL) {
		const char *failure_fd = getenv("DTEE_TEST_FCNTL_GETFL_FAILURE_FD");

		if (failure_fd != NULL && failure_fd[0] && fd == atoi(failure_fd)) {
			return true;
		}
	} else if (cmd == F_SETFL) {
		const char *failure_fd = getenv("DTEE_TEST_FCNTL_SETFL_FAILURE_FD");

		if (failure_fd != NULL && failure_fd[0] && fd == atoi(failure_fd)) {
			return true;
		}
	}

	return false;
}

static bool dtee_test_fcntl_match_non_blocking(int fd, int cmd) {
	if (cmd == F_GETFL) {
		if (fd == STDOUT_FILENO) {
			const char *non_blocking_stdout = getenv("DTEE_TEST_FCNTL_NON_BLOCKING_STDOUT");

			if (non_blocking_stdout != NULL) {
				return !strcmp(non_blocking_stdout, "1");
			}
		} else if (fd == STDERR_FILENO) {
			const char *non_blocking_stderr = getenv("DTEE_TEST_FCNTL_NON_BLOCKING_STDERR");

			if (non_blocking_stderr != NULL) {
				return !strcmp(non_blocking_stderr, "1");
			}
		}
	}

	return false;
}

static int dtee_test_fcntl_failure(int fd __attribute__((unused)), int cmd __attribute__((unused)), ...) {
	errno = EINVAL;
	return -1;
}

static int dtee_test_fcntl_return_non_blocking(int fd, int cmd, ...) {
	int (*next_fcntl)(int, int, ...) = TEST_FCN_NEXT(fcntl);
	int ret = next_fcntl(fd, cmd);

	if (ret != -1) {
		ret |= O_NONBLOCK;
	}

	return ret;
}

TEST_FCN_REPL(int, fcntl, (int fd, int cmd, ...)) {
	void *args = __builtin_apply_args();
	int (*next_fcntl)(int, int, ...) = TEST_FCN_NEXT(fcntl);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (dtee_test_fcntl_match_failure(fd, cmd)) {
				next_fcntl = dtee_test_fcntl_failure;
			} else if (dtee_test_fcntl_match_non_blocking(fd, cmd)) {
				next_fcntl = dtee_test_fcntl_return_non_blocking;
			}
		}

		active = false;
	}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-function-type"
	void *ret = __builtin_apply((void (*)())next_fcntl, args, 1024);
#pragma GCC diagnostic pop
	__builtin_return(ret);
}
