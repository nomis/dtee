#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "allow-n-times.h"
#include "is-dtee.h"
#include "is-fd-unix-socket.h"

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int (*next_connect)(int, const struct sockaddr *, socklen_t) = dlsym(RTLD_NEXT, "connect");

	if (dtee_test_is_dtee()) {
		if (dtee_test_is_fd_unix_socket(sockfd, NULL)) {
			static unsigned long allowed = 0;
			static bool first = true;

			if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_CONNECT_FAILURE_ALLOW", &first, &allowed)) {
				errno = EACCES;
				return -1;
			}
		}
	}

	return (*next_connect)(sockfd, addr, addrlen);
}
