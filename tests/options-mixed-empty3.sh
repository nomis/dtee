#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Empty command stops all following options parsing
run_test "--debug-options" "" "-o" "filename1" "-e" "filename2"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
