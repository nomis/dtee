#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-overwrite.txt"
}

run_test -C "$TESTDIR/$NAME.file.com-overwrite.txt" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.com-overwrite.txt}" "$TESTDIR/$NAME.file.com-overwrite.txt"
CMP_COM_O=$?

echo RET $RET
echo CMP_COM_O $CMP_COM_O
if [ $RET -eq 0 ] && [ $CMP_COM_O -eq 0 ]; then
	exit 0
fi
exit 1
