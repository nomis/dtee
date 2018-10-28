#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "is-dtee.h"
#include "is-fd-unix-socket.h"

static int dtee_test_close_failure(int fd __attribute__((unused))) {
	errno = EIO;
	return -1;
}

int close(int fd) {
	int (*next_close)(int) = dlsym(RTLD_NEXT, "close");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (dtee_test_is_fd_unix_socket(fd, NULL)) {
				if (dtee_test_is_ppid_dtee()) {
					if (getenv("DTEE_TEST_SOCKET_CLOSE_FAILURE_CHILD") != NULL) {
						next_close = dtee_test_close_failure;
					}
				} else {
					if (getenv("DTEE_TEST_SOCKET_CLOSE_FAILURE_PARENT") != NULL) {
						next_close = dtee_test_close_failure;
					}
				}
			}
		}

		active = false;
	}

	return (*next_close)(fd);
}
