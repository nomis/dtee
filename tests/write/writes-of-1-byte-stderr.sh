. "$(dirname -- "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.err-overwrite.txt"
}

run_test -E "$TESTDIR/$NAME.file.err-overwrite.txt" ./util/test-writes-of-n-bytes STDERR_FILENO 1 0
RET=$?

cmp_files "file.err-overwrite"
CMP_ERR_O=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_ERR_O 0
