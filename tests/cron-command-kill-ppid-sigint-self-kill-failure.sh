. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
	rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

TEST_LD_PRELOAD=(./libtest-kill-failure)
# If kill fails, we can only exit with a simulated SIGINT
run_test -q -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid $SIGINT
RET=$?

test_prepare
eval $(run_with_preload ./test-waitpid ./dtee ./dtee -q -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid $SIGINT)
RET2=$?

variables_must_eq RET $((128 + $SIGINT)) \
	RET2 0 \
	WIFSIGNALED 0
