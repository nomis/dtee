#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

TEST_EXEC=./cronty
run_test "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
