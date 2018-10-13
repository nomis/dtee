#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Command stops further options parsing
run_test "--debug-options" "command" "-o" "argument 1" "-x" "argument 2" "-y" "argument 3" "--help"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
