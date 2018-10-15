#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
        rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

# Disable core dumps (this may not work on Linux)
ulimit -H -c 0 || exit 1
ulimit -S -c 0 || exit 1
run_test -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid 3
RET=$?

test_prepare
eval $(./test-waitpid ./dtee ./dtee -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid 3)
RET2=$?

echo RET $RET
echo RET2 $RET
echo WIFEXITED $WIFEXITED
echo WEXITSTATUS $WEXITSTATUS
echo WIFSIGNALED $WIFSIGNALED
echo WTERMSIG $WTERMSIG
if [ $RET -eq $((128 + 3)) ] && [ $RET2 -eq 0 ] && [ $WIFSIGNALED -eq 1 ] && [ $WTERMSIG -eq 3 ]; then
	exit 0
fi
exit 1
