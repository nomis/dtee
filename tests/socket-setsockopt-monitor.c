#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <dlfcn.h>
#include <stdio.h>
#include <limits.h>

#include "is-dtee.h"

int setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen) {
	int (*next_setsockopt)(int, int, int, const void *, socklen_t) = dlsym(RTLD_NEXT, "setsockopt");

	if (dtee_test_is_dtee()) {
		struct sockaddr_storage addr;
		socklen_t addrlen = sizeof(addr);

		if (getsockname(sockfd, (struct sockaddr*)&addr, &addrlen) == 0) {
			if (addr.ss_family == AF_UNIX && level == SOL_SOCKET) {
				struct sockaddr_un *un_addr = (struct sockaddr_un*)&addr;
				const char *name = NULL;
				const char *value_str = NULL;
				int value = 0;

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
							un_addr->sun_path, name, value_str);
					} else {
						fprintf(stderr, "socket-setsockopt-monitor: setsockopt(\"%s\", SOL_SOCKET, %s, %u, ...)\n",
							un_addr->sun_path, name, value);
					}
					fflush(stderr);
				}
			}
		}
	}

	return (*next_setsockopt)(sockfd, level, optname, optval, optlen);
}
