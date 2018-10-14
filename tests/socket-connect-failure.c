#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "is-dtee.h"
#include "is-fd-unix-socket.h"

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int (*next_connect)(int, const struct sockaddr *, socklen_t) = dlsym(RTLD_NEXT, "connect");

	if (dtee_test_is_dtee()) {
		if (dtee_test_is_fd_unix_socket(sockfd, NULL)) {
			static unsigned long allowed = 0;
			static bool first = true;
			const char *allowed_str = getenv("DTEE_TEST_SOCKET_CONNECT_FAILURE_ALLOW");

			if (first) {
				if (allowed_str != NULL) {
					allowed = strtoul(allowed_str, NULL, 10);
				}
				first = false;
			}

			if (allowed) {
				--allowed;
			} else {
				errno = EACCES;
				return -1;
			}
		}
	}

	return (*next_connect)(sockfd, addr, addrlen);
}
