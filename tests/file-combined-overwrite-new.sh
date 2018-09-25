#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f "$TESTDIR/$NAME.file.com-overwrite"

run_test_once -C "$TESTDIR/$NAME.file.com-overwrite" "$RUN"
RET=$?

cmp "$TESTDIR/$NAME.file.com-overwrite" "${0/.sh/.file.com-overwrite}"
CMP_COM_O=$?
[ $CMP_COM_O -ne 0 ] && diff -U4 "${0/.sh/.file.com-overwrite}" "$TESTDIR/$NAME.file.com-overwrite"

echo RET $RET
echo CMP_COM_O $CMP_COM_O
if [ $RET -eq 0 ] && [ $CMP_COM_O -eq 0 ]; then
	exit 0
fi
exit 1
