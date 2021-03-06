. "$(dirname -- "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt"
}

# Writes of 0 bytes should work and not be considered EOF
run_test -q -O "$TESTDIR/$NAME.file.out-overwrite.txt" ./util/test-write-0-bytes STDOUT_FILENO $EXIT_FAILURE 0
RET=$?

cmp_files "file.out-overwrite"
CMP_OUT_O=$?

variables_must_eq RET $EXIT_FAILURE \
	CMP_OUT_O 0
