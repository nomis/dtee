#!/bin/bash
. "$(dirname "$0")"/util/common.sh

# Ignoring SIGINT does not affect behaviour when the child process receives SIGINT
run_test -i ./test-kill-pid 2
RET=$?

eval $(./test-waitpid ./dtee ./dtee -i ./test-kill-pid 2)
RET2=$?

variables_must_eq RET $((128 + 2)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG 2