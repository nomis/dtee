#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-append.txt"
}

run_test -o "$TESTDIR/$NAME.file.out-append.txt" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.out-append.txt}" "$TESTDIR/$NAME.file.out-append.txt"
CMP_OUT_A=$?

echo RET $RET
echo CMP_OUT_A $CMP_OUT_A
if [ $RET -eq 0 ] && [ $CMP_OUT_A -eq 0 ]; then
	exit 0
fi
exit 1
