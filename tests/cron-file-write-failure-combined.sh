. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-append.txt"
}

export DTEE_TEST_FILE_WRITE_FAIL_NAME="$TESTDIR/$NAME.file.com-append.txt"

TEST_LD_PRELOAD=(test-file-write-failure)
run_test -q -c "$TESTDIR/$NAME.file.com-append.txt" "$RUN"
RET=$?

cmp_files "file.com-append"
CMP_COM_A=$?

variables_must_eq RET $EX_IOERR \
	CMP_COM_A 0
