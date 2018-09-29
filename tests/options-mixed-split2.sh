#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# End of options signifier stops all following options parsing, even if the next word is an option
run_test_once "--debug-options" "-o" "filename1" "-e" "filename2" "--" "-c" "filename3" "command"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
