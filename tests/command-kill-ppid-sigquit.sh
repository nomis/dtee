#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
        rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

# Disable core dumps (this may not have the desired effect on Linux)
ulimit -H -c 0 || exit $TEST_EX_SKIP
ulimit -S -c 0 || exit $TEST_EX_SKIP
run_test -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid 3
RET=$?

test_prepare
eval $(./test-waitpid ./dtee ./dtee -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid 3)
RET2=$?

variables_must_eq RET $((128 + 3)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG 3
