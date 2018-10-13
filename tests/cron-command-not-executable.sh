#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f cron_command_not_executable
(umask 0111; touch cron_command_not_executable)

run_test_once "-q" "./cron_command_not_executable"
RET=$?

echo RET $RET
if [ $RET -eq 66 ]; then
	exit 0
fi
exit 1
