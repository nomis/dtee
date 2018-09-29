#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f command_not_executable
(umask 0111; touch command_not_executable)

run_test_once "./command_not_executable"
RET=$?

echo RET $RET
if [ $RET -eq 66 ]; then
	exit 0
fi
exit 1
