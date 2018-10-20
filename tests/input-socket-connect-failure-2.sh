#!/bin/bash
. "$(dirname "$0")"/util/common.sh

# Allow the first socket to work
export DTEE_TEST_SOCKET_CONNECT_FAILURE_ALLOW=1

TEST_LD_PRELOAD="./libtest-socket-connect-failure.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
