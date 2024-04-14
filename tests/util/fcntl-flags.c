#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
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
	assert(cmd == F_GETFL);
	int (*next_fcntl)(int, int, ...) = TEST_FCN_NEXT(fcntl);
	int ret = next_fcntl(fd, cmd);

	if (ret != -1) {
		ret |= O_NONBLOCK;
	}

	return ret;
}

TEST_FCN_REPL(int, fcntl, (int fd, int cmd, ...)) {
	va_list ap;
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

	va_start(ap, cmd);
	int ret;

	switch (cmd) {
		// (void)
	case F_GETFD:
	case F_GETFL:
#ifdef F_GETOWN
	case F_GETOWN:
#endif
#ifdef F_GETSIG
	case F_GETSIG:
#endif
#ifdef F_GETLEASE
	case F_GETLEASE:
#endif
#ifdef F_GETPIPE_SZ
	case F_GETPIPE_SZ:
#endif
#ifdef F_GET_SEALS
	case F_GET_SEALS:
#endif
		ret = next_fcntl(fd, cmd);
		break;

		// (int)
	case F_DUPFD:
#ifdef F_DUPFD_CLOEXEC
	case F_DUPFD_CLOEXEC:
#endif
	case F_SETFD:
	case F_SETFL:
#ifdef F_SETOWN
	case F_SETOWN:
#endif
#ifdef F_SETSIG
	case F_SETSIG:
#endif
#ifdef F_SETLEASE
	case F_SETLEASE:
#endif
#ifdef F_NOTIFY
	case F_NOTIFY:
#endif
#ifdef F_SETPIPE_SZ
	case F_SETPIPE_SZ:
#endif
#ifdef F_ADD_SEALS
	case F_ADD_SEALS:
#endif
		ret = next_fcntl(fd, cmd, va_arg(ap, int));
		break;

		// (void*)
	case F_SETLK:
	case F_SETLKW:
	case F_GETLK:
#ifdef F_OFD_SETLK
	case F_OFD_SETLK:
#endif
#ifdef F_OFD_SETLKW
	case F_OFD_SETLKW:
#endif
#ifdef F_OFD_GETLK
	case F_OFD_GETLK:
#endif
#ifdef F_GETOWN_EX
	case F_GETOWN_EX:
#endif
#ifdef F_SETOWN_EX
	case F_SETOWN_EX:
#endif
#ifdef F_GET_RW_HINT
	case F_GET_RW_HINT:
#endif
#ifdef F_SET_RW_HINT
	case F_SET_RW_HINT:
#endif
#ifdef F_GET_FILE_RW_HINT
	case F_GET_FILE_RW_HINT:
#endif
#ifdef F_SET_FILE_RW_HINT
	case F_SET_FILE_RW_HINT:
#endif
		ret = next_fcntl(fd, cmd, va_arg(ap, void *));
		break;

	default:
		fprintf(stderr, "unhandled fcntl(fd=%d, cmd=%d, ...)\n", fd, cmd);
		fflush(stderr);
		abort();
	}

	int errno_copy = errno;
	va_end(ap);
	errno = errno_copy;
	return ret;
}
