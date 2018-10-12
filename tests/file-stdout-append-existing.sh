#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f "$TESTDIR/$NAME.file.out-append.txt"
echo Existing line 1 >"$TESTDIR/$NAME.file.out-append.txt"
echo Existing line 2 >>"$TESTDIR/$NAME.file.out-append.txt"
echo Existing line 3 >>"$TESTDIR/$NAME.file.out-append.txt"

run_test_once -o "$TESTDIR/$NAME.file.out-append.txt" "$RUN"
RET=$?

cmp "$TESTDIR/$NAME.file.out-append.txt" "${0/.sh/.file.out-append.txt}"
CMP_OUT_A=$?
[ $CMP_OUT_A -ne 0 ] && diff -U4 "${0/.sh/.file.out-append.txt}" "$TESTDIR/$NAME.file.out-append.txt"

echo RET $RET
echo CMP_OUT_A $CMP_OUT_A
if [ $RET -eq 0 ] && [ $CMP_OUT_A -eq 0 ]; then
	exit 0
fi
exit 1
