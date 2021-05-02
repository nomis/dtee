. "$(dirname "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.canary"
	FIFO=$(make_fifo "out")
	./util/test-closed-pipe-reader "$TESTDIR/$NAME.canary" <"$FIFO" &
}

FIFO=$(make_fifo "out")
TEST_ALT_STDOUT="$FIFO"
run_test -q ./util/test-closed-pipe-writer "$TESTDIR/$NAME.canary" "$RUN"
RET=$?

TEST_WAITPID_CLOSED_STDOUT=1 eval_ret ./util/test-waitpid ./dtee ./dtee -q "$RUN"
RET2=$?

variables_must_eq RET $((128 + $SIGPIPE)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGPIPE
