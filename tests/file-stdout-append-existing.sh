#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f "$TESTDIR/$NAME.file.out-append"
echo Existing line 1 >"$TESTDIR/$NAME.file.out-append"
echo Existing line 2 >>"$TESTDIR/$NAME.file.out-append"
echo Existing line 3 >>"$TESTDIR/$NAME.file.out-append"

run_test_once -o "$TESTDIR/$NAME.file.out-append" "$RUN"
RET=$?

cmp "$TESTDIR/$NAME.file.out-append" "${0/.sh/.file.out-append}"
CMP_OUT_A=$?
[ $CMP_OUT_A -ne 0 ] && diff -U4 "${0/.sh/.file.out-append}" "$TESTDIR/$NAME.file.out-append"

echo RET $RET
echo CMP_OUT_A $CMP_OUT_A
if [ $RET -eq 0 ] && [ $CMP_OUT_A -eq 0 ]; then
	exit 0
fi
exit 1
