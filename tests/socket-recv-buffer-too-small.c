#include <sys/socket.h>
#include <sys/types.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>

#include "is-dtee.h"
#include "is-fd-unix-socket.h"

#define MAX_BUFFER_SIZE 16

static ssize_t dtee_test_recv_failure(int sockfd, void *buf, size_t len, int flags) {
	ssize_t (*next_recv)(int, void *, size_t, int) = dlsym(RTLD_NEXT, "recv");

	if (len > MAX_BUFFER_SIZE) {
		len = MAX_BUFFER_SIZE;
	}

	return (*next_recv)(sockfd, buf, len, flags);
}

static ssize_t dtee_test_recvfrom_failure(int sockfd, void *buf, size_t len, int flags, struct sockaddr *src_addr, socklen_t *addrlen) {
	ssize_t (*next_recvfrom)(int, void *, size_t, int, struct sockaddr*, socklen_t *) = dlsym(RTLD_NEXT, "recvfrom");

	if (len > MAX_BUFFER_SIZE) {
		len = MAX_BUFFER_SIZE;
	}

	return (*next_recvfrom)(sockfd, buf, len, flags, src_addr, addrlen);
}

static ssize_t dtee_test_recvmsg_failure(int sockfd, struct msghdr *msg, int flags) {
	ssize_t (*next_recvmsg)(int, struct msghdr *, int) = dlsym(RTLD_NEXT, "recvmsg");
	size_t len = MAX_BUFFER_SIZE;

	for (size_t i = 0; i < msg->msg_iovlen; i++) {
		if (msg->msg_iov[i].iov_len > len) {
			msg->msg_iov[i].iov_len = len;
			len = 0;
		} else {
			len -= msg->msg_iov[i].iov_len;
		}
	}

	return (*next_recvmsg)(sockfd, msg, flags);
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
