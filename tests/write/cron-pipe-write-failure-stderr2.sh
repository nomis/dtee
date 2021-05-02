. "$(dirname "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.canary"
	FIFO=$(make_fifo "err")
	./util/test-closed-pipe-reader "$TESTDIR/$NAME.canary" <"$FIFO" &
}

FIFO=$(make_fifo "err")
TEST_ALT_STDERR="$FIFO"
run_test -q ./util/test-closed-pipe-writer "$TESTDIR/$NAME.canary" "$RUN"
RET=$?

TEST_WAITPID_CLOSED_STDERR=1 eval_ret ./util/test-waitpid ./dtee ./dtee -q "$RUN"
RET2=$?

# If SIGPIPE is received while outputting the final report of the process
# status, the signal is ignored so the return value will be unchanged.
variables_must_eq RET 42 \
	RET2 0 \
	WIFSIGNALED 0
