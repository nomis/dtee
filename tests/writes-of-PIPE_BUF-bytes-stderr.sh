#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.err-overwrite.txt"
}

run_test -E "$TESTDIR/$NAME.file.err-overwrite.txt" ./test-writes-of-n-bytes STDERR_FILENO PIPE_BUF 0
RET=$?

cmp "$TESTDIR/$NAME.file.err-overwrite.txt" "${0/.sh/.file.err-overwrite.txt}"
CMP_ERR_O=$?
[ $CMP_ERR_O -ne 0 ] && diff -U4 "${0/.sh/.file.err-overwrite.txt}" "$TESTDIR/$NAME.file.err-overwrite.txt"

echo RET $RET
echo CMP_ERR_O $CMP_ERR_O
if [ $RET -eq 0 ] && [ $CMP_ERR_O -eq 0 ]; then
	exit 0
fi
exit 1
