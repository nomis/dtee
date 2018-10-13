#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD="./libsocket-bind-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 69 ]; then
	exit 0
fi
exit 1
