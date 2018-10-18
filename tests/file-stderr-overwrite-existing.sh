#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.err-overwrite"
	echo Existing line >"$TESTDIR/$NAME.file.err-overwrite"
}

run_test -E "$TESTDIR/$NAME.file.err-overwrite" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.err-overwrite}" "$TESTDIR/$NAME.file.err-overwrite"
CMP_ERR_O=$?

echo RET $RET
echo CMP_ERR_O $CMP_ERR_O
if [ $RET -eq 0 ] && [ $CMP_ERR_O -eq 0 ]; then
	exit 0
fi
exit 1
