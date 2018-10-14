#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "allow-n-times.h"
#include "is-dtee.h"

int socket(int domain, int type, int protocol) {
	int (*next_socket)(int, int, int) = dlsym(RTLD_NEXT, "socket");

	if (dtee_test_is_dtee()) {
		if (domain == AF_UNIX) {
			static unsigned long allowed = 0;
			static bool first = true;

			if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_UNIX_FAILURE_ALLOW", &first, &allowed)) {
				errno = EAFNOSUPPORT;
				return -1;
			}
		}
	}

	return (*next_socket)(domain, type, protocol);
}
