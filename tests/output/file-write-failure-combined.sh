. "$(dirname -- "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-append.txt"
}

export DTEE_TEST_FILE_FAIL_NAME="$TESTDIR/$NAME.file.com-append.txt"
export DTEE_TEST_FILE_OPEN_FAIL=0
export DTEE_TEST_FILE_WRITE_FAIL=all
export DTEE_TEST_FILE_CLOSE_FAIL=0

TEST_LD_PRELOAD=(test-file-failures)
run_test -c "$TESTDIR/$NAME.file.com-append.txt" "$RUN"
RET=$?

cmp_files "file.com-append"
CMP_COM_A=$?

variables_must_eq RET $EX_IOERR \
	CMP_COM_A 0
