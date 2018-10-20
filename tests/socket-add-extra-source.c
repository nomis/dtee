#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "is-dtee.h"
#include "is-fd-unix-socket.h"

static int extra_fd = -1;

// When dtee connects to its own input socket, clone the socket details and make an extra socket
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
	int (*next_connect)(int, const struct sockaddr *, socklen_t) = dlsym(RTLD_NEXT, "connect");

	if (dtee_test_is_dtee() && extra_fd < 0) {
		struct sockaddr_un sock_addr;

		if (dtee_test_is_fd_unix_socket(sockfd, &sock_addr)) {
			char path[sizeof(sock_addr.sun_path) + 1] = { 0 };

			memcpy(path, &sock_addr.sun_path, sizeof(sock_addr.sun_path));
			path[strlen(path) - 1] = 'x';
			strncpy(sock_addr.sun_path, path, sizeof(sock_addr.sun_path) - 1);

			extra_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
			if (extra_fd >= 0) {
				if (!bind(extra_fd, &sock_addr, sizeof(sock_addr))) {
					if (!connect(extra_fd, addr, addrlen)) {
						printf("socket-add-extra-source: created socket \"%s\" connected to \"%s\"\n",
							sock_addr.sun_path, path);
						fflush(stdout);
					} else {
						perror("socket-add-extra-source: connect");
						fflush(stderr);

						close(extra_fd);
						extra_fd = -1;
					}

					if (unlink(path)) {
						perror("socket-add-extra-source: unlink");
						fflush(stderr);
					}
				} else {
					perror("socket-add-extra-source: bind");
					fflush(stderr);

					close(extra_fd);
					extra_fd = -1;
				}
			} else {
				perror("socket-add-extra-source: socket");
				fflush(stderr);
			}
		}
	}

	return (*next_connect)(sockfd, addr, addrlen);
}

// When dtee is about to exec the command, insert an additional message
int execvp(const char *filename, char *const argv[]) {
	int (*next_execvp)(const char*, char * const *) = dlsym(RTLD_NEXT, "execvp");

	if (dtee_test_is_dtee()) {
		if (extra_fd >= 0) {
			const char *message = "Message from unknown source\n";

			printf("socket-add-extra-source: inserting additional message\n");
			fflush(stdout);

			int len = strlen(message);
			if (write(extra_fd, message, strlen(message)) != len) {
				perror("socket-add-extra-source: write");
				fflush(stderr);
			} else {
				printf("socket-add-extra-source: inserted additional message\n");
				fflush(stdout);
			}
		} else {
			fprintf(stderr, "socket-add-extra-source: no socket available\n");
			fflush(stderr);
		}
	}

	return (*next_execvp)(filename, argv);
}
