#!/bin/bash
. "$(dirname "$0")"/util/common.sh

TEST_CLOSED_STDERR=1
run_test "$RUN"
RET=$?

eval $(TEST_WAITPID_CLOSED_STDERR=1 ./test-waitpid ./dtee ./dtee "$RUN")
RET2=$?

variables_must_eq RET $((128 + 13)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG 13
