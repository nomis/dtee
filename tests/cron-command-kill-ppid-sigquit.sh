#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Disable core dumps (this may not work on Linux)
ulimit -H -c 0 || exit 1
ulimit -S -c 0 || exit 1
run_test -q ./test-kill-ppid 3
RET=$?

eval $(./test-waitpid ./dtee ./dtee -q ./test-kill-ppid 3)
RET2=$?

echo RET $RET
echo RET2 $RET
echo WIFEXITED $WIFEXITED
echo WEXITSTATUS $WEXITSTATUS
echo WIFSIGNALED $WIFSIGNALED
echo WTERMSIG $WTERMSIG
if [ $RET -eq $((128 + 3)) ] && [ $RET2 -eq 0 ] && [ $WIFSIGNALED -eq 1 ] && [ $WTERMSIG -eq 3 ]; then
	exit 0
fi
exit 1
