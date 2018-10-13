#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# If the input socket bind fails, execute the command without dtee processing
TEST_LD_PRELOAD="./libsocket-bind-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "-q" "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
