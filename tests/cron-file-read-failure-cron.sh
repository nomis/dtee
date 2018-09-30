#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD="./libmkostemp-writeonly.so:./libfake-getpid.so"
run_test_once "-q" "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 74 ]; then
	exit 0
fi
exit 1
