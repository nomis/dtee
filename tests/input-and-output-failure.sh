#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD="./libsocket-unix-failure.so:./libfake-getpid.so:./libfake-getuid.so"
(umask 0333; touch file_not_writeable)
run_test "-o" "file_not_writeable" "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 73 ]; then
	exit 0
fi
exit 1
