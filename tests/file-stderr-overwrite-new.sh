#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f "$TESTDIR/$NAME.file.err-overwrite"

run_test_once -E "$TESTDIR/$NAME.file.err-overwrite" "$RUN"
RET=$?

cmp "$TESTDIR/$NAME.file.err-overwrite" "${0/.sh/.file.err-overwrite}"
CMP_ERR_O=$?
[ $CMP_ERR_O -ne 0 ] && diff -U4 "${0/.sh/.file.err-overwrite}" "$TESTDIR/$NAME.file.err-overwrite"

echo RET $RET
echo CMP_ERR_O $CMP_ERR_O
if [ $RET -eq 0 ] && [ $CMP_ERR_O -eq 0 ]; then
	exit 0
fi
exit 1
