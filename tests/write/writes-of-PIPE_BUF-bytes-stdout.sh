. "$(dirname "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt"
}

run_test -O "$TESTDIR/$NAME.file.out-overwrite.txt" ./util/test-writes-of-n-bytes STDOUT_FILENO PIPE_BUF 0
RET=$?

cmp_files "file.out-overwrite"
CMP_OUT_O=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_OUT_O 0
