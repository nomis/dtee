#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Numbers are out of order to check they're not being sorted
run_test_once "--debug-options" "command" "argument 3" "argument 1" "argument 2"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
