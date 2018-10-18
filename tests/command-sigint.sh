#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test ./test-kill-pid 2
RET=$?

eval $(./test-waitpid ./dtee ./dtee ./test-kill-pid 2)
RET2=$?

variables_must_eq RET $((128 + 2)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG 2
