#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "allow-n-times.h"
#include "is-dtee.h"
#include "is-fd-unix-socket.h"
#include "dtee-fcn.h"

TEST_FCN_DECL(ssize_t, recv, (int sockfd, void *buf, size_t len, int flags));
TEST_FCN_DECL(ssize_t, recvfrom, (int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen));
TEST_FCN_DECL(ssize_t, recvmsg, (int sockfd, struct msghdr *msg, int flags));

static ssize_t dtee_test_recv_failure(int sockfd __attribute__((unused)), void *buf __attribute__((unused)), size_t len __attribute__((unused)), int flags __attribute__((unused))) {
	ssize_t (*next_recv)(int, void *, size_t, int) = TEST_FCN_NEXT(recv);
	static unsigned long allowed = 0;
	static bool first = true;
	ssize_t ret = (*next_recv)(sockfd, buf, len, flags);

	if (ret == -1) {
		// EAGAIN/EWOULDBLOCK doesn't count
	} else if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_RECV_FAILURE_ALLOW", &first, &allowed)) {
		errno = ENOTCONN;
		ret = -1;
	}
	return ret;
}

static ssize_t dtee_test_recvfrom_failure(int sockfd  __attribute__((unused)), void *buf  __attribute__((unused)), size_t len  __attribute__((unused)), int flags  __attribute__((unused)), struct sockaddr *src_addr  __attribute__((unused)), socklen_t *addrlen __attribute__((unused))) {
	ssize_t (*next_recvfrom)(int, void *, size_t, int, struct sockaddr *, socklen_t *) = TEST_FCN_NEXT(recvfrom);
	static unsigned long allowed = 0;
	static bool first = true;
	ssize_t ret = (*next_recvfrom)(sockfd, buf, len, flags, src_addr, addrlen);

	if (ret == -1) {
		// EAGAIN/EWOULDBLOCK doesn't count
	} else if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_RECV_FAILURE_ALLOW", &first, &allowed)) {
		errno = ENOTCONN;
		ret = -1;
	}
	return ret;
}

static ssize_t dtee_test_recvmsg_failure(int sockfd  __attribute__((unused)), struct msghdr *msg  __attribute__((unused)), int flags __attribute__((unused))) {
	ssize_t (*next_recvmsg)(int, struct msghdr *, int) = TEST_FCN_NEXT(recvmsg);
	static unsigned long allowed = 0;
	static bool first = true;
	ssize_t ret = (*next_recvmsg)(sockfd, msg, flags);

	if (ret == -1) {
		// EAGAIN/EWOULDBLOCK doesn't count
	} else if (!dtee_test_allow_n_times("DTEE_TEST_SOCKET_RECV_FAILURE_ALLOW", &first, &allowed)) {
		errno = ENOTCONN;
		ret = -1;
	}
	return ret;
}

TEST_FCN_REPL(ssize_t, recv, (int sockfd, void *buf, size_t len, int flags)) {
	ssize_t (*next_recv)(int, void *, size_t, int) = TEST_FCN_NEXT(recv);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (dtee_test_is_fd_unix_socket(sockfd, NULL)) {
				next_recv = dtee_test_recv_failure;
			}
		}

		active = false;
	}

	return (*next_recv)(sockfd, buf, len, flags);
}

TEST_FCN_REPL(ssize_t, recvfrom, (int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen)) {
	ssize_t (*next_recvfrom)(int, void *, size_t, int, struct sockaddr *, socklen_t *) = TEST_FCN_NEXT(recvfrom);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (dtee_test_is_fd_unix_socket(sockfd, NULL)) {
				next_recvfrom = dtee_test_recvfrom_failure;
			}
		}

		active = false;
	}

	return (*next_recvfrom)(sockfd, buf, len, flags, src_addr, addrlen);
}

TEST_FCN_REPL(ssize_t, recvmsg, (int sockfd, struct msghdr *msg, int flags)) {
	ssize_t (*next_recvmsg)(int, struct msghdr *, int) = TEST_FCN_NEXT(recvmsg);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			if (dtee_test_is_fd_unix_socket(sockfd, NULL)) {
				next_recvmsg = dtee_test_recvmsg_failure;
			}
		}

		active = false;
	}

	return (*next_recvmsg)(sockfd, msg, flags);
}
