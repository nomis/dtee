#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f "$TESTDIR/$NAME.file.err-append"
echo Existing line 1 >"$TESTDIR/$NAME.file.err-append"
echo Existing line 2 >>"$TESTDIR/$NAME.file.err-append"
echo Existing line 3 >>"$TESTDIR/$NAME.file.err-append"

run_test_once -e "$TESTDIR/$NAME.file.err-append" "$RUN"
RET=$?

cmp "$TESTDIR/$NAME.file.err-append" "${0/.sh/.file.err-append}"
CMP_ERR_A=$?
[ $CMP_ERR_A -ne 0 ] && diff -U4 "${0/.sh/.file.err-append}" "$TESTDIR/$NAME.file.err-append"

echo RET $RET
echo CMP_ERR_A $CMP_ERR_A
if [ $RET -eq 0 ] && [ $CMP_ERR_A -eq 0 ]; then
	exit 0
fi
exit 1
