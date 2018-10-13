#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test "--debug-options" "--all" "--your" "--base" "--are" "--belong" "--to" "--us"
RET=$?

echo RET $RET
if [ $RET -eq 64 ]; then
	exit 0
fi
exit 1
