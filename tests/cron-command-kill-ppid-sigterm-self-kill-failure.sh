. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
	 rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

TEST_LD_PRELOAD="./libtest-kill-failure.so"
# If kill fails, we can only exit with a simulated SIGTERM
run_test -q -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid 15
RET=$?

test_prepare
eval $(LD_PRELOAD="$TEST_LD_PRELOAD" ./test-waitpid ./dtee ./dtee -q -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid 15)
RET2=$?

variables_must_eq RET $((128 + $SIGTERM)) \
	RET2 0 \
	WIFSIGNALED 0
