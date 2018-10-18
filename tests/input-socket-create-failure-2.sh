#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Allow the first socket to work
export DTEE_TEST_SOCKET_UNIX_FAILURE_ALLOW=1

TEST_LD_PRELOAD="./libsocket-unix-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
