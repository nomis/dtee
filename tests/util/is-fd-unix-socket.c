#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>
#include <string.h>

#include "is-fd-unix-socket.h"

bool dtee_test_is_fd_unix_socket(int fd, struct sockaddr_un *addr) {
	struct sockaddr_un buf = { .sun_family = AF_UNSPEC };
	socklen_t addrlen = sizeof(*addr);
	bool is_unix_socket = false;
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (addr == NULL) {
			addr = &buf;
		} else {
			memset(addr, 0, sizeof(*addr));
			addr->sun_family = AF_UNSPEC;
		}

		if (getsockname(fd, (struct sockaddr*)addr, &addrlen) == 0) {
			if (addr->sun_family == AF_UNIX) {
				is_unix_socket = true;
#ifdef __DragonFly__
			// Doesn't return the socket address for unbound Unix sockets
			// Fixed by ddec9e61ea71e07fc90922027c433c850db5a3ad
			} else if (addrlen == 0) {
				is_unix_socket = true;
#endif
			}
		}

		active = false;
	}

	return is_unix_socket;
}
