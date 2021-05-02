. "$(dirname "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.canary"
	FIFO=$(make_fifo "file")
	./util/test-closed-pipe-reader "$TESTDIR/$NAME.canary" <"$FIFO" &
}

FIFO=$(make_fifo "file")
run_test -o "$FIFO" ./util/test-closed-pipe-writer "$TESTDIR/$NAME.canary" "$RUN"
RET=$?

test_prepare
eval_ret ./util/test-waitpid ./dtee ./dtee -o "$FIFO" ./util/test-closed-pipe-writer "$TESTDIR/$NAME.canary" "$RUN"
RET2=$?

variables_must_eq RET $((128 + $SIGPIPE)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGPIPE
