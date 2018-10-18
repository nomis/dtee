#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt"
}

run_test -q -O "$TESTDIR/$NAME.file.out-overwrite.txt" ./test-writes-of-n-bytes STDOUT_FILENO PIPE_BUF 1
RET=$?

cmp_files "${0/.sh/.file.out-overwrite.txt}" "$TESTDIR/$NAME.file.out-overwrite.txt"
CMP_OUT_O=$?

echo RET $RET
echo CMP_OUT_O $CMP_OUT_O
if [ $RET -eq 1 ] && [ $CMP_OUT_O -eq 0 ]; then
	exit 0
fi
exit 1
