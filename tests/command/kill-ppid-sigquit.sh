. "$(dirname "$0")"/../util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
	rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

# Disable core dumps (this may not have the desired effect on Linux
# a program handles core dumps, but this won't affect the test script)
ulimit -S -c 0 || exit $TEST_EX_SKIP
ulimit -H -c 0 || exit $TEST_EX_SKIP
run_test -O "$DTEE_TEST_MONITOR_OUTPUT" ./util/test-kill-ppid $SIGQUIT
RET=$?

test_prepare
eval_ret ./util/test-waitpid ./dtee ./dtee -O "$DTEE_TEST_MONITOR_OUTPUT" ./util/test-kill-ppid $SIGQUIT
RET2=$?

variables_must_eq RET $((128 + $SIGQUIT)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGQUIT
