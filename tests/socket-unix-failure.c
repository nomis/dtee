#define __socket30 __dtee__hide___socket30
#define socket __dtee__hide__socket
#define setsockopt __dtee__hide__setsockopt

#include <sys/socket.h>
#include <sys/types.h>
#include <assert.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#undef socket
#undef __socket30
#undef setsockopt

#include "allow-n-times.h"
#include "is-dtee.h"
#include "dtee-fcn.h"

static int dtee_test_socket_failure(int domain __attribute__((unused)), int type __attribute__((unused)), int protocol __attribute__((unused))) {
	errno = EAFNOSUPPORT;
	return -1;
}

TEST_FCN_REPL(int, socket, (int domain, int type, int protocol)) {
	int (*next_socket)(int, int, int) = TEST_FCN_NEXT(socket);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (domain == AF_UNIX) {
				static unsigned long allowed = 0;
				static bool first = true;

				if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_UNIX_FAILURE_ALLOW", &first, &allowed)) {
					next_socket = dtee_test_socket_failure;
				}
			}
		}

		active = false;
	}

	return (*next_socket)(domain, type, protocol);
}

#if defined(__NetBSD__)
TEST_FCN_REPL(int, __socket30, (int domain, int type, int protocol)) {
	int (*next___socket30)(int, int, int) = TEST_FCN_NEXT(__socket30);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (domain == AF_UNIX) {
				static unsigned long allowed = 0;
				static bool first = true;

				if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_UNIX_FAILURE_ALLOW", &first, &allowed)) {
					next___socket30 = dtee_test_socket_failure;
				}
			}
		}

		active = false;
	}

	return (*next___socket30)(domain, type, protocol);
}
#endif

#if defined(__MACH__) && defined(__APPLE__) || defined(__FreeBSD__)
static int dtee_test_setsockopt_failure(int sockfd __attribute__((unused)), int level __attribute__((unused)), int optname __attribute__((unused)), void *optval __attribute__((unused)), socklen_t *optlen __attribute__((unused))) {
	assert(errno == EAFNOSUPPORT);
	return -1;
}

TEST_FCN_REPL(int, setsockopt, (int sockfd, int level, int optname, void *optval, socklen_t *optlen)) {
	int (*next_setsockopt)(int, int, int, void *, socklen_t *) = TEST_FCN_NEXT(setsockopt);

	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (errno == EAFNOSUPPORT
					&& sockfd == -1
					&& level == SOL_SOCKET
					&& optname == SO_NOSIGPIPE) {
				// Workaround Boost.Asio (1.74.0) bug on Darwin
				// and FreeBSD where it calls setsockopt() on
				// new sockets even if socket() returned -1
				// https://github.com/boostorg/asio/pull/368
				next_setsockopt = dtee_test_setsockopt_failure;
			}
		}

		active = false;
	}

	return (*next_setsockopt)(sockfd, level, optname, optval, optlen);
}
#endif
