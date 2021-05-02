. "$(dirname "$0")"/../util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
	rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

run_test -q -O "$DTEE_TEST_MONITOR_OUTPUT" ./util/test-kill-ppid $SIGHUP
RET=$?

test_prepare
eval_ret ./util/test-waitpid ./dtee ./dtee -q -O "$DTEE_TEST_MONITOR_OUTPUT" ./util/test-kill-ppid $SIGHUP
RET2=$?

variables_must_eq RET $((128 + $SIGHUP)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGHUP
