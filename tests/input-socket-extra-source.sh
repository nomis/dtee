#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

COMMON_TEST_LD_PRELOAD=("${COMMON_TEST_LD_PRELOAD[@]/.\/libexecvp-fd-check.so}")

TEST_LD_PRELOAD="./libsocket-add-extra-source.so:./libmkdtemp-consistent.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "$RUN"
RET=$?

echo RET $RET
if [ $RET -eq 0 ]; then
	exit 0
fi
exit 1
