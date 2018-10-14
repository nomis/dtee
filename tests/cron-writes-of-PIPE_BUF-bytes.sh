#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt"
}

run_test -q -O "$TESTDIR/$NAME.file.out-overwrite.txt" ./test-writes-of-n-bytes PIPE_BUF 1
RET=$?

cmp "$TESTDIR/$NAME.file.out-overwrite.txt" "${0/.sh/.file.out-overwrite.txt}"
CMP_OUT_O=$?
[ $CMP_OUT_O -ne 0 ] && diff -U4 "${0/.sh/.file.out-overwrite.txt}" "$TESTDIR/$NAME.file.out-overwrite.txt"

echo RET $RET
echo CMP_OUT_O $CMP_OUT_O
if [ $RET -eq 1 ] && [ $CMP_OUT_O -eq 0 ]; then
	exit 0
fi
exit 1
