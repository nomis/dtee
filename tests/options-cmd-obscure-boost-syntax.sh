#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Check that obscure Boost syntax "--=..." works as expected 😩
run_test_once "--debug-options" "-o" "stdout_append" "--=command" "-e" "stderr_append" "--=argument 1" "-c" "combined_append" "--=argument 2" "-q" "--=argument 3"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
