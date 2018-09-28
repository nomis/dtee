#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test_once \
	"-o" "stdout_append" \
	"-e" "stderr_append" \
	"-c" "combined_append" \
	"-O" "stdout_overwrite" \
	"-E" "stderr_overwrite" \
	"-C" "combined_overwrite" \
	"--debug-options"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
