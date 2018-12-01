. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.err-append.txt"
}

export DTEE_TEST_FILE_WRITE_FAIL_NAME="$TESTDIR/$NAME.file.err-append.txt"

TEST_LD_PRELOAD=(test-file-write-failure)
run_test -q -e "$TESTDIR/$NAME.file.err-append.txt" "$RUN"
RET=$?

cmp_files "file.err-append"
CMP_ERR_A=$?

variables_must_eq RET $EX_IOERR \
	CMP_ERR_A 0
