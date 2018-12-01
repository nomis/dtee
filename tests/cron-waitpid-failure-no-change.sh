. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
	rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

export DTEE_TEST_WAITPID_FAILURE_TYPE=NO_CHANGE
TEST_LD_PRELOAD=(test-waitpid-failure)
run_test -q -C "$DTEE_TEST_MONITOR_OUTPUT" ./test-lorem-ipsum-wait-exit 0
RET=$?

test_prepare
eval $(run_with_preload ./test-waitpid ./dtee ./dtee -q -C "$DTEE_TEST_MONITOR_OUTPUT" ./test-lorem-ipsum-wait-exit 0)
RET2=$?

variables_must_eq RET $((128 + $SIGCHLD)) \
	RET2 0 \
	WIFSIGNALED 0
