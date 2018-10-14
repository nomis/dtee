#ifndef DTEE_IS_FD_UNIX_SOCKET_H_
#define DTEE_IS_FD_UNIX_SOCKET_H_

#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>

bool dtee_test_is_fd_unix_socket(int fd, struct sockaddr_un *addr);

#endif
