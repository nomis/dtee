#define __socket30 __dtee__hide___socket30
#define socket __dtee__hide__socket

#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#undef socket
#undef __socket30

#include "allow-n-times.h"
#include "is-dtee.h"

static int dtee_test_socket_failure(int domain __attribute__((unused)), int type __attribute__((unused)), int protocol __attribute__((unused))) {
	errno = EAFNOSUPPORT;
	return -1;
}

int socket(int domain, int type, int protocol) {
	int (*next_socket)(int, int, int) = dlsym(RTLD_NEXT, "socket");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (domain == AF_UNIX) {
				static unsigned long allowed = 0;
				static bool first = true;

				if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_UNIX_FAILURE_ALLOW", &first, &allowed)) {
					next_socket = dtee_test_socket_failure;
				}
			}
		}

		active = false;
	}

	return (*next_socket)(domain, type, protocol);
}

int __socket30(int domain, int type, int protocol) {
	int (*next___socket30)(int, int, int) = dlsym(RTLD_NEXT, "__socket30");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (domain == AF_UNIX) {
				static unsigned long allowed = 0;
				static bool first = true;

				if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_UNIX_FAILURE_ALLOW", &first, &allowed)) {
					next___socket30 = dtee_test_socket_failure;
				}
			}
		}

		active = false;
	}

	return (*next___socket30)(domain, type, protocol);
}
