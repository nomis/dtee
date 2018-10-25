. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.canary"
	FIFO=$(make_fifo "file")
	./test-closed-pipe-reader "$TESTDIR/$NAME.canary" <"$FIFO" &
}

FIFO=$(make_fifo "file")
run_test -q -o "$FIFO" ./test-closed-pipe-writer "$TESTDIR/$NAME.canary" "$RUN"
RET=$?

test_prepare
eval $(./test-waitpid ./dtee ./dtee -q -o "$FIFO" ./test-closed-pipe-writer "$TESTDIR/$NAME.canary" "$RUN")
RET2=$?

variables_must_eq RET $((128 + 13)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG 13
