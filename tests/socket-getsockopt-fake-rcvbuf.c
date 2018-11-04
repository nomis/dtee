#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "is-dtee.h"
#include "is-fd-unix-socket.h"

#define max(a, b) (((a) > (b)) ? (a) : (b))

static int dtee_test_fake_rcvbuf(int sockfd __attribute__((unused)), int level __attribute__((unused)), int optname __attribute__((unused)), void *optval, socklen_t *optlen __attribute__((unused))) {
	const char *value_str = getenv("DTEE_TEST_GETSOCKOPT_RCVBUF");
	int *value = (int*)optval;

	if (value_str != NULL) {
		if (!strcmp(value_str, "max(PIPE_BUF,BUFSIZ)*4")) {
			errno = 0;
			*value = max(PIPE_BUF, BUFSIZ)*4;
		} else if (!strcmp(value_str, "max(PIPE_BUF,BUFSIZ)*2")) {
			errno = 0;
			*value = max(PIPE_BUF, BUFSIZ)*2;
		} else if (!strcmp(value_str, "max(PIPE_BUF,BUFSIZ)")) {
			errno = 0;
			*value = max(PIPE_BUF, BUFSIZ);
		} else if (!strcmp(value_str, "max(PIPE_BUF,BUFSIZ)/2")) {
			errno = 0;
			*value = max(PIPE_BUF, BUFSIZ)/2;
		} else if (!strcmp(value_str, "max(PIPE_BUF,BUFSIZ)/4")) {
			errno = 0;
			*value = max(PIPE_BUF, BUFSIZ)/4;
		} else {
			errno = 0;
			*value = strtoul(value_str, NULL, 10);
		}

#ifdef __linux__
		// Boost (1.62) is going to divide the value by 2, so
		// multiply it in advance to get the expected value.
		*value *= 2;
#endif

		return 0;
	} else {
		errno = EINVAL;
		return -1;
	}
}

int getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen) {
	int (*next_getsockopt)(int, int, int, void *, socklen_t *) = dlsym(RTLD_NEXT, "getsockopt");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (dtee_test_is_fd_unix_socket(sockfd, NULL)) {
				if (level == SOL_SOCKET && optname == SO_RCVBUF && *optlen == sizeof(int)) {
					next_getsockopt = dtee_test_fake_rcvbuf;
				}
			}
		}

		active = false;
	}

	return (*next_getsockopt)(sockfd, level, optname, optval, optlen);
}
