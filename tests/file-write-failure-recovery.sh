#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite1.txt" "$TESTDIR/$NAME.file.out-overwrite2.txt"
}

export DTEE_TEST_FILE_WRITE_FAIL_NAME="$TESTDIR/$NAME.file.out-overwrite1.txt"
export DTEE_FILE_WRITE_FAILURE_EXIT_CODE=0

TEST_LD_PRELOAD="./libfile-write-failure-recovery.so"
run_test -O "$TESTDIR/$NAME.file.out-overwrite1.txt" -O "$TESTDIR/$NAME.file.out-overwrite2.txt" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.out-overwrite1.txt}" "$TESTDIR/$NAME.file.out-overwrite1.txt"
CMP_OUT1_O=$?

cmp_files "${0/.sh/.file.out-overwrite2.txt}" "$TESTDIR/$NAME.file.out-overwrite2.txt"
CMP_OUT2_O=$?

echo RET $RET
echo CMP_OUT1_O $CMP_OUT1_O
echo CMP_OUT2_O $CMP_OUT2_O
if [ $RET -eq 74 ] && [ $CMP_OUT1_O -eq 0 ] && [ $CMP_OUT2_O ]; then
	exit 0
fi
exit 1
