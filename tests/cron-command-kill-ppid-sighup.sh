#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test -q ./test-kill-ppid 1
RET=$?

eval $(./test-waitpid ./dtee ./dtee -q ./test-kill-ppid 1)
RET2=$?

echo RET $RET
echo RET2 $RET
echo WIFEXITED $WIFEXITED
echo WEXITSTATUS $WEXITSTATUS
echo WIFSIGNALED $WIFSIGNALED
echo WTERMSIG $WTERMSIG
if [ $RET -eq $((128 + 1)) ] && [ $RET2 -eq 0 ] && [ $WIFSIGNALED -eq 1 ] && [ $WTERMSIG -eq 1 ]; then
	exit 0
fi
exit 1
