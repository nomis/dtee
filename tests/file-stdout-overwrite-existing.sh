#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f "$TESTDIR/$NAME.file.out-overwrite.txt"
echo Existing line >"$TESTDIR/$NAME.file.out-overwrite.txt"

run_test_once -O "$TESTDIR/$NAME.file.out-overwrite.txt" "$RUN"
RET=$?

cmp "$TESTDIR/$NAME.file.out-overwrite.txt" "${0/.sh/.file.out-overwrite.txt}"
CMP_OUT_O=$?
[ $CMP_OUT_O -ne 0 ] && diff -U4 "${0/.sh/.file.out-overwrite.txt}" "$TESTDIR/$NAME.file.out-overwrite.txt"

echo RET $RET
echo CMP_OUT_O $CMP_OUT_O
if [ $RET -eq 0 ] && [ $CMP_OUT_O -eq 0 ]; then
	exit 0
fi
exit 1
