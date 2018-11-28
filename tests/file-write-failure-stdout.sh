. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-append.txt"
}

export DTEE_TEST_FILE_WRITE_FAIL_NAME="$TESTDIR/$NAME.file.out-append.txt"

TEST_LD_PRELOAD=(./libtest-file-write-failure)
run_test -o "$TESTDIR/$NAME.file.out-append.txt" "$RUN"
RET=$?

cmp_files "file.out-append"
CMP_OUT_A=$?

variables_must_eq RET $EX_IOERR \
	CMP_OUT_A 0
