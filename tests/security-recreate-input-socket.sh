. "$(dirname "$0")"/util/common.sh

export DTEE_TEST_MONITOR_OUTPUT="$TESTDIR/$NAME.canary"

function test_prepare() {
	rm -f "$DTEE_TEST_MONITOR_OUTPUT"
}

no_ld_preload "./libtest-execvp-fd-check.so"
TEST_EXTRA_OUTPUT=1

TEST_LD_PRELOAD="./libtest-mkdtemp-consistent.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
# It should not be possible to recreate the input socket and receive output from a command
run_test -O "$DTEE_TEST_MONITOR_OUTPUT" ./test-security-recreate-input-socket
RET=$?

variables_must_eq RET $((128 + $SIGTERM))

