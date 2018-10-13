#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Empty command stops all following options parsing, even if they're empty
run_test "--debug-options" "" "-o" "" "-e" "filename"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
