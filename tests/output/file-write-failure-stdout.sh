. "$(dirname -- "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-append.txt"
}

export DTEE_TEST_FILE_FAIL_NAME="$TESTDIR/$NAME.file.out-append.txt"
export DTEE_TEST_FILE_OPEN_FAIL=0
export DTEE_TEST_FILE_WRITE_FAIL=all
export DTEE_TEST_FILE_CLOSE_FAIL=0

TEST_LD_PRELOAD=(test-file-failures)
run_test -o "$TESTDIR/$NAME.file.out-append.txt" "$RUN"
RET=$?

cmp_files "file.out-append"
CMP_OUT_A=$?

variables_must_eq RET $EX_IOERR \
	CMP_OUT_A 0
