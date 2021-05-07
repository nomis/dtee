#pragma once

#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>

bool dtee_test_is_fd_unix_socket(int fd, struct sockaddr_un *addr);
