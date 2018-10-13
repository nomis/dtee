#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# End of options signifier stops all following options parsing
run_test "--debug-options" "-o" "filename1" "-e" "filename2" "--" "command" "-c" "filename3" "another argument"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
