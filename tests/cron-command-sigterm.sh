#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test "-q" ./test-kill-pid 15
RET=$?

eval $(./test-waitpid ./dtee ./dtee "-q" ./test-kill-pid 15)
RET2=$?

echo RET $RET
echo RET2 $RET
echo WIFEXITED $WIFEXITED
echo WEXITSTATUS $WEXITSTATUS
echo WIFSIGNALED $WIFSIGNALED
echo WTERMSIG $WTERMSIG
if [ $RET -eq $((128 + 15)) ] && [ $RET2 -eq 0 ] && [ $WIFSIGNALED -eq 0 ]; then
	exit 0
fi
exit 1
