#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "is-dtee.h"

int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int (*next_bind)(int, const struct sockaddr *, socklen_t) = dlsym(RTLD_NEXT, "bind");

	if (dtee_test_is_dtee()) {
		if (addrlen >= ((u_int8_t*)&addr->sa_family - (u_int8_t*)addr) + sizeof(addr->sa_family)) {
			if (addr->sa_family == AF_UNIX) {
				static unsigned long allowed = 0;
				static bool first = true;
				const char *allowed_str = getenv("DTEE_TEST_SOCKET_BIND_FAILURE_ALLOW");

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
	}

	return (*next_bind)(sockfd, addr, addrlen);
}
