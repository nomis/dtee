. "$(dirname -- "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt"
}

run_test -q -O "$TESTDIR/$NAME.file.out-overwrite.txt" ./util/test-writes-of-n-bytes STDOUT_FILENO "max(PIPE_BUF,BUFSIZ)" 1
RET=$?

cmp_files "file.out-overwrite"
CMP_OUT_O=$?

variables_must_eq RET $EXIT_FAILURE \
	CMP_OUT_O 0
