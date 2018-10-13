#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test "--debug-options" "-a" "-y" "-b" "-a" "-b" "-t" "-u"
RET=$?

echo RET $RET
if [ $RET -eq 64 ]; then
	exit 0
fi
exit 1
