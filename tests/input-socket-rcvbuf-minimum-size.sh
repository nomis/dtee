#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_GETSOCKOPT_RCVBUF="PIPE_BUF"

# If the outgoing SO_RCVBUF is exactly PIPE_BUF, it does not need to be modified
# The incoming SO_SNDBUF will be set to the same as the observed SO_RCVBUF
TEST_LD_PRELOAD="./libsocket-setsockopt-monitor.so:./libsocket-getsockopt-fake-rcvbuf.so:./libmkdtemp-consistent.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
