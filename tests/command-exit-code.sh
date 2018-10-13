#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 42 ]; then
	exit 0
fi
exit 1
