#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Increase core size to unlimited and then the command will disable core dumps
ulimit -H -c unlimited || exit 1
ulimit -S -c unlimited || exit 1
export DTEE_TEST_RLIMIT_CORE=0

run_test ./test-kill-pid 3
RET=$?

eval $(./test-waitpid ./dtee ./dtee ./test-kill-pid 3)
RET2=$?

echo RET $RET
echo RET2 $RET
echo WIFEXITED $WIFEXITED
echo WEXITSTATUS $WEXITSTATUS
echo WIFSIGNALED $WIFSIGNALED
echo WTERMSIG $WTERMSIG
if [ $RET -eq $((128 + 3)) ] && [ $RET2 -eq 0 ] && [ $WIFSIGNALED -eq 0 ]; then
	exit 0
fi
exit 1
