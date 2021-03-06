#include <sys/socket.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>

#include "allow-n-times.h"
#include "is-dtee.h"
#include "is-fd-unix-socket.h"
#include "dtee-fcn.h"

static int dtee_test_shutdown_failure(int sockfd __attribute__((unused)), int how __attribute__((unused))) {
	errno = ENOTCONN;
	return -1;
}

TEST_FCN_REPL(int, shutdown, (int sockfd, int how)) {
	int (*next_shutdown)(int, int) = TEST_FCN_NEXT(shutdown);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (dtee_test_is_fd_unix_socket(sockfd, NULL)) {
				static unsigned long allowed = 0;
				static bool first = true;

				if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_SHUTDOWN_FAILURE_ALLOW", &first, &allowed)) {
					next_shutdown = dtee_test_shutdown_failure;
				}
			}
		}

		active = false;
	}

	return (*next_shutdown)(sockfd, how);
}
