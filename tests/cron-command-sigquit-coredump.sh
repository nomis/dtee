#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Increase core size to unlimited and then the command will set it again
ulimit -H -c unlimited || exit 1
ulimit -S -c unlimited || exit 1
export DTEE_TEST_RLIMIT_CORE=RLIM_INFINITY

run_test "-q" ./test-kill-pid 3
RET=$?

eval $(./test-waitpid ./dtee ./dtee "-q" ./test-kill-pid 3)
RET2=$?

variables_must_eq RET $((128 + 3)) \
	RET2 0 \
	WIFSIGNALED 0
