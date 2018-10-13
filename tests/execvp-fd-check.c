#include <dlfcn.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "is-dtee.h"

static void dtee_test_check_fds(void) {
	for (int fd = 0; fd < 4096; fd++) {
		int desc_flags = fcntl(fd, F_GETFD);
		int status_flags = fcntl(fd, F_GETFL);
		bool open = (desc_flags != -1) && (status_flags != -1);
		bool cloexec = (desc_flags & FD_CLOEXEC) != 0;

		switch (fd) {
		case STDIN_FILENO:
		case STDOUT_FILENO:
		case STDERR_FILENO:
			if (!open || cloexec) {
				fprintf(stderr, "fd %d is not open\n", fd);
				abort();
			}
			break;

		default:
			if (open && !cloexec) {
				fprintf(stderr, "fd %d is open and will not be closed on exec\n", fd);
				abort();
			}
			break;
		}
	}
}

int execvp(const char *filename, char *const argv[]) {
	int (*next_execvp)(const char*, char * const *) = dlsym(RTLD_NEXT, "execvp");

	if (dtee_test_is_dtee()) {
		dtee_test_check_fds();
	}

	return (*next_execvp)(filename, argv);
}
