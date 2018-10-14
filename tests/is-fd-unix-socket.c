#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>
#include <string.h>

#include "is-fd-unix-socket.h"

bool dtee_test_is_fd_unix_socket(int fd, struct sockaddr_un *addr) {
	struct sockaddr_un buf = { .sun_family = AF_UNSPEC };
	socklen_t addrlen = sizeof(*addr);

	if (addr == NULL) {
		addr = &buf;
	}

	if (getsockname(fd, (struct sockaddr*)addr, &addrlen) == 0) {
		if (addr->sun_family == AF_UNIX) {
			return true;
		}
	}

	return false;
}
