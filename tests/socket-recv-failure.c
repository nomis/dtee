#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "allow-n-times.h"
#include "is-dtee.h"
#include "is-fd-unix-socket.h"

static ssize_t dtee_test_recv_failure(int sockfd __attribute__((unused)), void *buf __attribute__((unused)), size_t len __attribute__((unused)), int flags __attribute__((unused))) {
	ssize_t (*next_recv)(int, void *, size_t, int) = dlsym(RTLD_NEXT, "recv");
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
	ssize_t (*next_recvfrom)(int, void *, size_t, int, struct sockaddr*, socklen_t *) = dlsym(RTLD_NEXT, "recvfrom");
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
	ssize_t (*next_recvmsg)(int, struct msghdr *, int) = dlsym(RTLD_NEXT, "recvmsg");
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

ssize_t recv(int sockfd, void *buf, size_t len, int flags) {
	ssize_t (*next_recv)(int, void *, size_t, int) = dlsym(RTLD_NEXT, "recv");

	if (dtee_test_is_dtee()) {
		if (dtee_test_is_fd_unix_socket(sockfd, NULL)) {
			next_recv = dtee_test_recv_failure;
		}
	}

	return (*next_recv)(sockfd, buf, len, flags);
}

ssize_t recvfrom(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
	ssize_t (*next_recvfrom)(int, void *, size_t, int, struct sockaddr*, socklen_t *) = dlsym(RTLD_NEXT, "recvfrom");

	if (dtee_test_is_dtee()) {
		if (dtee_test_is_fd_unix_socket(sockfd, NULL)) {
			next_recvfrom = dtee_test_recvfrom_failure;
		}
	}

	return (*next_recvfrom)(sockfd, buf, len, flags, src_addr, addrlen);
}

ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags) {
	ssize_t (*next_recvmsg)(int, struct msghdr *, int) = dlsym(RTLD_NEXT, "recvmsg");

	if (dtee_test_is_dtee()) {
		if (dtee_test_is_fd_unix_socket(sockfd, NULL)) {
			next_recvmsg = dtee_test_recvmsg_failure;
		}
	}

	return (*next_recvmsg)(sockfd, msg, flags);
}
