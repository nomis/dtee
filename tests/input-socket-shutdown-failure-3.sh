#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Allow the first two sockets to work
export DTEE_TEST_SOCKET_SHUTDOWN_FAILURE_ALLOW=2

TEST_LD_PRELOAD="./libsocket-shutdown-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
