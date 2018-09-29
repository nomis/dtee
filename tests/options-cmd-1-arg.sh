#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test_once "--debug-options" "command" "argument 1"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
