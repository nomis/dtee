. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
	 rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

run_test -q -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid 15
RET=$?

test_prepare
eval $(./test-waitpid ./dtee ./dtee -q -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-kill-ppid 15)
RET2=$?

variables_must_eq RET $((128 + 15)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG 15
