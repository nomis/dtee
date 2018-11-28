#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "is-dtee.h"
#include "is-fd-unix-socket.h"
#include "dtee-fcn.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

static int extra_fd = -1;

// When dtee connects to its own input socket, clone the socket details and make an extra socket
TEST_FCN_REPL(int, connect, (int sockfd, const struct sockaddr *addr, socklen_t addrlen)) {
	int (*next_connect)(int, const struct sockaddr *, socklen_t) = TEST_FCN_NEXT(connect);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee() && extra_fd < 0) {
			struct sockaddr_un sock_addr = { .sun_family = AF_UNSPEC, .sun_path = { 0 } };
			struct sockaddr_un extra_addr;
			struct sockaddr_un dest_addr = { .sun_family = AF_UNSPEC, .sun_path = { 0 } };

			if (dtee_test_is_fd_unix_socket(sockfd, &sock_addr)) {
				sock_addr.sun_path[sizeof(sock_addr.sun_path) - 1] = 0;
				extra_addr = sock_addr;
				extra_addr.sun_path[strlen(extra_addr.sun_path) - 1] = 'x';
				memcpy(&dest_addr, addr, min(sizeof(dest_addr), addrlen));
				dest_addr.sun_path[sizeof(dest_addr.sun_path) - 1] = 0;

				extra_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
				if (extra_fd >= 0) {
					if (!bind(extra_fd, (struct sockaddr*)&extra_addr, sizeof(extra_addr))) {
						if (!connect(extra_fd, addr, addrlen)) {
							printf("socket-add-extra-source: created socket \"%s\" connected to \"%s\"\n",
								extra_addr.sun_path, dest_addr.sun_path);
							fflush(stdout);
						} else {
							perror("socket-add-extra-source: connect");
							fflush(stderr);

							close(extra_fd);
							extra_fd = -1;
						}

						if (unlink(extra_addr.sun_path)) {
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

		active = false;
	}

	return (*next_connect)(sockfd, addr, addrlen);
}

// When dtee is about to exec the command, insert an additional message
TEST_FCN_REPL(int, execvp, (const char *filename, char *const argv[])) {
	int (*next_execvp)(const char*, char * const *) = TEST_FCN_NEXT(execvp);
	static __thread bool active = false;

	if (!active) {
		active = true;

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

		active = false;
	}

	return (*next_execvp)(filename, argv);
}
