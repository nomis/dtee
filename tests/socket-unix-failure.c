#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "is-dtee.h"

int socket(int domain, int type, int protocol) {
	int (*next_socket)(int, int, int) = dlsym(RTLD_NEXT, "socket");

	if (dtee_test_is_dtee()) {
		if (domain == AF_UNIX) {
			static unsigned long allowed = 0;
			static bool first = true;
			const char *allowed_str = getenv("DTEE_TEST_SOCKET_UNIX_FAILURE_ALLOW");

			if (first) {
				if (allowed_str != NULL) {
					allowed = strtoul(allowed_str, NULL, 10);
				}
				first = false;
			}

			if (allowed) {
				--allowed;
			} else {
				errno = EAFNOSUPPORT;
				return -1;
			}
		}
	}

	return (*next_socket)(domain, type, protocol);
}
