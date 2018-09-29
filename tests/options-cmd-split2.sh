#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# End of options signifier stops all following options parsing, even with empty command
run_test_once "--debug-options" "--" ""
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
