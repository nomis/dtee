#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "allow-n-times.h"
#include "is-dtee.h"

static int dtee_test_socket_failure(int domain __attribute__((unused)), int type __attribute__((unused)), int protocol __attribute__((unused))) {
	errno = EAFNOSUPPORT;
	return -1;
}

int socket(int domain, int type, int protocol) {
	int (*next_socket)(int, int, int) = dlsym(RTLD_NEXT, "socket");

	if (dtee_test_is_dtee()) {
		if (domain == AF_UNIX) {
			static unsigned long allowed = 0;
			static bool first = true;

			if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_UNIX_FAILURE_ALLOW", &first, &allowed)) {
				next_socket = dtee_test_socket_failure;
			}
		}
	}

	return (*next_socket)(domain, type, protocol);
}
