. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
	rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

run_test -q -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid $SIGTERM
RET=$?

test_prepare
eval $(./test-waitpid ./dtee ./dtee -q -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid $SIGTERM)
RET2=$?

variables_must_eq RET $((128 + $SIGTERM)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGTERM
