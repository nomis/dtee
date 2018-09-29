#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Empty filename followed by empty command with empty argument
run_test_once "--debug-options" "-o" "" "" ""
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
