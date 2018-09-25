#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f "$TESTDIR/$NAME.file.out-overwrite"

run_test_once -O "$TESTDIR/$NAME.file.out-overwrite" "$RUN"
RET=$?

cmp "$TESTDIR/$NAME.file.out-overwrite" "${0/.sh/.file.out-overwrite}"
CMP_OUT_O=$?
[ $CMP_OUT_O -ne 0 ] && diff -U4 "${0/.sh/.file.out-overwrite}" "$TESTDIR/$NAME.file.out-overwrite"

echo RET $RET
echo CMP_OUT_O $CMP_OUT_O
if [ $RET -eq 0 ] && [ $CMP_OUT_O -eq 0 ]; then
	exit 0
fi
exit 1
