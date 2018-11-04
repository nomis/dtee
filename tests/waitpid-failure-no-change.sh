. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
	rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

export DTEE_TEST_WAITPID_FAILURE_TYPE=NO_CHANGE
TEST_LD_PRELOAD="./libtest-waitpid-failure.so"
run_test -C "$DTEE_TEST_MONITOR_OUTPUT" ./test-lorem-ipsum-wait-exit 1
RET=$?

test_prepare
eval $(LD_PRELOAD="$TEST_LD_PRELOAD" ./test-waitpid ./dtee ./dtee -C "$DTEE_TEST_MONITOR_OUTPUT" ./test-lorem-ipsum-wait-exit 1)
RET2=$?

variables_must_eq RET $((128 + $SIGCHLD)) \
	RET2 0 \
	WIFSIGNALED 0
