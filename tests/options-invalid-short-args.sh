#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test_once "--debug-options" "-a" "your" "-b" "are" "-b" "to" "us"
RET=$?

echo RET $RET
if [ $RET -eq 64 ]; then
	exit 0
fi
exit 1
