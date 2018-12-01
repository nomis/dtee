. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file1.txt" "$TESTDIR/$NAME.file2.txt"
	make_fifo "block"
}

no_ld_preload "test-execvp-fd-check"

# Send SIGINT to the process while write() is blocked
# The signal should be handled (and ignored) without interrupting the write()
FIFO=$(make_fifo "block")
run_test ./test-fifo-write-interrupt ./dtee -i "$TESTDIR/$NAME.file1.txt" "$FIFO" "$TESTDIR/$NAME.file2.txt"
RET=$?

cmp_files "file1"
CMP_FILE1=$?

cmp_files "file2"
CMP_FILE2=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_FILE1 0 \
	CMP_FILE2 0
