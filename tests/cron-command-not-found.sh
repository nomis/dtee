#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test "-q" "./command_not_found"
RET=$?

echo RET $RET
if [ $RET -eq 66 ]; then
	exit 0
fi
exit 1
