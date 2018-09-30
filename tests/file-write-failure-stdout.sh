#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test_once "-o" "/dev/full" "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 74 ]; then
	exit 0
fi
exit 1