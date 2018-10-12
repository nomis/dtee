#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test_once ./test-kill-ppid 3
RET=$?

echo RET $RET
if [ $RET -eq $((128 + 3)) ]; then
	exit 0
fi
exit 1
