#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test_once ./test-kill-ppid 15
RET=$?

echo RET $RET
if [ $RET -eq $((128 + 15)) ]; then
	exit 0
fi
exit 1
