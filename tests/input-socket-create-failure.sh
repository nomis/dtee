#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD="./libsocket-unix-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test_once "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 71 ]; then
	exit 0
fi
exit 1
