#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test "-q" "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq $((128 + 15)) ]; then
	exit 0
fi
exit 1
