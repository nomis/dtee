#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>

#include "is-dtee.h"

int socket(int domain, int type, int protocol) {
	int (*next_socket)(int, int, int) = dlsym(RTLD_NEXT, "socket");

	if (dtee_test_is_dtee()) {
		if (domain == AF_UNIX) {
			errno = EAFNOSUPPORT;
			return -1;
		}
	}

	return (*next_socket)(domain, type, protocol);
}
