#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# If the temporary output file cannot be opened, it should fallback to using stdout/stderr as normal
TEST_LD_PRELOAD="./libmkostemp-failure.so:./libfake-getpid.so"
run_test_once "-q" "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
