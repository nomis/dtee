#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite1.txt" "$TESTDIR/$NAME.file.out-overwrite2.txt"
}

export DTEE_TEST_FILE_WRITE_FAIL_NAME="$TESTDIR/$NAME.file.out-overwrite1.txt"
export DTEE_FILE_WRITE_FAILURE_EXIT_CODE=0

TEST_LD_PRELOAD="./libfile-write-failure-recovery.so"
run_test -q -O "$TESTDIR/$NAME.file.out-overwrite1.txt" -O "$TESTDIR/$NAME.file.out-overwrite2.txt" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.out-overwrite1.txt}" "$TESTDIR/$NAME.file.out-overwrite1.txt"
CMP_OUT1_O=$?

cmp_files "${0/.sh/.file.out-overwrite2.txt}" "$TESTDIR/$NAME.file.out-overwrite2.txt"
CMP_OUT2_O=$?

variables_must_eq RET $EX_IOERR \
	CMP_OUT1_O 0 \
	CMP_OUT2_O 0
