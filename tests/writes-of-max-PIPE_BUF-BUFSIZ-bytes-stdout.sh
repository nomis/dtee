. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt"
}

run_test -O "$TESTDIR/$NAME.file.out-overwrite.txt" ./test-writes-of-n-bytes STDOUT_FILENO "max(PIPE_BUF,BUFSIZ)" 0
RET=$?

cmp_files "file.out-overwrite"
CMP_OUT_O=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_OUT_O 0
