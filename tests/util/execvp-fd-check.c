#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

static void dtee_test_check_fds(void) {
	for (int fd = 0; fd < 4096; fd++) {
		int desc_flags = fcntl(fd, F_GETFD);
		int status_flags = fcntl(fd, F_GETFL);
		bool open = (desc_flags != -1) && (status_flags != -1);
		bool blocking = (status_flags & O_NONBLOCK) == 0;
		bool cloexec = (desc_flags & FD_CLOEXEC) != 0;

		switch (fd) {
		case STDIN_FILENO:
		case STDOUT_FILENO:
		case STDERR_FILENO:
			if (!open || cloexec) {
				fprintf(stderr, "execvp-fd-check: fd %d is not open\n", fd);
				fflush(stderr);
				abort();
			}
			break;

		default:
			if (open && !cloexec) {
				fprintf(stderr, "execvp-fd-check: fd %d is open and will not be closed on exec\n", fd);
				fflush(stderr);
				abort();
			}
			break;
		}

		switch (fd) {
		case STDOUT_FILENO:
		case STDERR_FILENO:
			if (!blocking) {
				fprintf(stderr, "execvp-fd-check: fd %d is not blocking\n", fd);
				fflush(stderr);
				abort();
			}
			break;

		default:
			break;
		}
	}
}

TEST_FCN_REPL(int, execvp, (const char *filename, char *const argv[])) {
	int (*next_execvp)(const char*, char * const *) = TEST_FCN_NEXT(execvp);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			dtee_test_check_fds();
		}

		active = false;
	}

	return (*next_execvp)(filename, argv);
}
