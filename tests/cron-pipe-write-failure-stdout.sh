. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.canary"
	FIFO=$(make_fifo "out")
	./test-closed-pipe-reader "$TESTDIR/$NAME.canary" <"$FIFO" &
}

FIFO=$(make_fifo "out")
TEST_ALT_STDOUT="$FIFO"
run_test -q ./test-closed-pipe-writer "$TESTDIR/$NAME.canary" "$RUN"
RET=$?

eval $(TEST_WAITPID_CLOSED_STDOUT=1 ./test-waitpid ./dtee ./dtee -q "$RUN")
RET2=$?

variables_must_eq RET $((128 + $SIGPIPE)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGPIPE
