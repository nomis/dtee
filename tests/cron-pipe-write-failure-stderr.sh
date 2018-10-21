#!/bin/bash
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.canary"
	FIFO=$(make_fifo "err")
	./test-closed-pipe-reader "$TESTDIR/$NAME.canary" <"$FIFO" &
}

FIFO=$(make_fifo "err")
TEST_ALT_STDERR="$FIFO"
run_test -q ./test-closed-pipe-writer "$TESTDIR/$NAME.canary" "$RUN"
RET=$?

eval $(TEST_WAITPID_CLOSED_STDERR=1 ./test-waitpid ./dtee ./dtee -q "$RUN")
RET2=$?

variables_must_eq RET $((128 + 13)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG 13
