#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-append.txt"
	echo Existing line 1 >"$TESTDIR/$NAME.file.com-append.txt"
	echo Existing line 2 >>"$TESTDIR/$NAME.file.com-append.txt"
	echo Existing line 3 >>"$TESTDIR/$NAME.file.com-append.txt"
}

run_test -c "$TESTDIR/$NAME.file.com-append.txt" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.com-append.txt}" "$TESTDIR/$NAME.file.com-append.txt"
CMP_COM_A=$?

echo RET $RET
echo CMP_COM_A $CMP_COM_A
if [ $RET -eq 0 ] && [ $CMP_COM_A -eq 0 ]; then
	exit 0
fi
exit 1
