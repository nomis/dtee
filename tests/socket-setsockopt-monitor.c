#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "is-dtee.h"
#include "is-fd-unix-socket.h"

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
	int (*next_setsockopt)(int, int, int, const void *, socklen_t) = dlsym(RTLD_NEXT, "setsockopt");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			struct sockaddr_un addr = { .sun_family = AF_UNSPEC, .sun_path = { 0 } };

			if (dtee_test_is_fd_unix_socket(sockfd, &addr)) {
				if (level == SOL_SOCKET) {
					const char *name = NULL;
					const char *value_str = NULL;
					int value = 0;

					addr.sun_path[sizeof(addr.sun_path) - 1] = 0;

					if (optname == SO_RCVBUF && optlen == sizeof(int)) {
						name = "SO_RCVBUF";
						value = *(int*)optval;
					} else if (optname == SO_SNDBUF && optlen == sizeof(int)) {
						name = "SO_SNDBUF";
						value = *(int*)optval;
					}

					if (value == PIPE_BUF*16) {
						value_str = "PIPE_BUF*16";
					} else if (value == PIPE_BUF*8) {
						value_str = "PIPE_BUF*8";
					} else if (value == PIPE_BUF*4) {
						value_str = "PIPE_BUF*4";
					} else if (value == PIPE_BUF*2) {
						value_str = "PIPE_BUF*2";
					} else if (value == PIPE_BUF) {
						value_str = "PIPE_BUF";
					} else if (value == PIPE_BUF/2) {
						value_str = "PIPE_BUF/2";
					} else if (value == PIPE_BUF/4) {
						value_str = "PIPE_BUF/4";
					} else if (value == PIPE_BUF/8) {
						value_str = "PIPE_BUF/8";
					} else if (value == PIPE_BUF/16) {
						value_str = "PIPE_BUF/16";
					}

					if (name != NULL) {
						if (value_str != NULL) {
							fprintf(stderr, "socket-setsockopt-monitor: setsockopt(\"%s\", SOL_SOCKET, %s, %s, ...)\n",
								addr.sun_path, name, value_str);
						} else {
							fprintf(stderr, "socket-setsockopt-monitor: setsockopt(\"%s\", SOL_SOCKET, %s, %u, ...)\n",
								addr.sun_path, name, value);
						}
						fflush(stderr);
					}
				}
			}
		}

		active = false;
	}

	return (*next_setsockopt)(sockfd, level, optname, optval, optlen);
}
