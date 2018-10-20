#!/bin/bash
. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_GETSOCKOPT_RCVBUF="PIPE_BUF/4"

# If the outgoing SO_RCVBUF is smaller than PIPE_BUF, it should be raised to at least PIPE_BUF
# The incoming SO_SNDBUF will be set to the same as the observed SO_RCVBUF
TEST_LD_PRELOAD="./libtest-socket-setsockopt-monitor.so:./libtest-socket-getsockopt-fake-rcvbuf.so:./libtest-mkdtemp-consistent.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
