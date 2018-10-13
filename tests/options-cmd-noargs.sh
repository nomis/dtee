#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test "--debug-options" "command"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
