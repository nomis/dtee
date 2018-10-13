#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test_once "$RUN"
RET=$?

eval $(./test-waitpid ./dtee ./dtee "$RUN")
RET2=$?

echo RET $RET
echo RET2 $RET
echo WIFEXITED $WIFEXITED
echo WEXITSTATUS $WEXITSTATUS
echo WIFSIGNALED $WIFSIGNALED
echo WTERMSIG $WTERMSIG
if [ $RET -eq $((128 + 9)) ] && [ $RET2 -eq 0 ] && [ $WIFSIGNALED -eq 0 ]; then
	exit 0
fi
exit 1
