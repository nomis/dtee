#!/bin/bash
. "$(dirname "$0")"/util/common.sh

# Allow the first two sockets to work
export DTEE_TEST_SOCKET_UNIX_FAILURE_ALLOW=2

TEST_LD_PRELOAD="./libtest-socket-unix-failure.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
