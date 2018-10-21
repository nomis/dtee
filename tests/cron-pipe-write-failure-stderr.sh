#!/bin/bash
. "$(dirname "$0")"/util/common.sh

TEST_CLOSED_STDERR=1
run_test -q "$RUN"
RET=$?

eval $(TEST_WAITPID_CLOSED_STDERR=1 ./test-waitpid ./dtee ./dtee -q "$RUN")
RET2=$?

variables_must_eq RET 0 \
	RET2 0 \
	WIFSIGNALED 0
