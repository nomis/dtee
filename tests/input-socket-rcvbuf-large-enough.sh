#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_GETSOCKOPT_RCVBUF="PIPE_BUF*4"

# If the outgoing SO_RCVBUF is larger than PIPE_BUF, it does not need to be modified
# The incoming SO_SNDBUF will be set to the same as the observed SO_RCVBUF
TEST_LD_PRELOAD="./libsocket-setsockopt-monitor.so:./libsocket-getsockopt-fake-rcvbuf.so:./libmkdtemp-consistent.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
