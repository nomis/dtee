. "$(dirname -- "$0")"/../util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
	rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

no_ld_preload "test-execvp-fd-check"
TEST_EXTRA_OUTPUT=1

TEST_LD_PRELOAD=(test-mkdtemp-consistent test-fake-getpid test-fake-getuid)
# It should not be possible to recreate the input socket and receive output from a command
run_test -O "$DTEE_TEST_MONITOR_OUTPUT" ./util/test-security-recreate-input-socket
RET=$?

variables_must_eq RET $((128 + $SIGTERM))

