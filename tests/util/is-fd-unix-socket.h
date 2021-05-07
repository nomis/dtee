#pragma once

#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool dtee_test_is_fd_unix_socket(int fd, struct sockaddr_un *addr);

#ifdef __cplusplus
}
#endif
