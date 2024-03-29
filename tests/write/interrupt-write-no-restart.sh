. "$(dirname -- "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file1.txt" "$TESTDIR/$NAME.file2.txt"
	make_fifo "block"
}

no_ld_preload "test-execvp-fd-check"
no_ld_preload "test-sigaction-restart-check"

# Disable the use of SA_RESTART to prove that it is having the desired effect
TEST_LD_PRELOAD=(test-sigaction-no-restart)

# Send SIGINT to the process while write() is blocked
# The signal should interrupt the write()
FIFO=$(make_fifo "block")
run_test ./util/test-fifo-write-interrupt ./dtee -i "$TESTDIR/$NAME.file1.txt" "$FIFO" "$TESTDIR/$NAME.file2.txt"
RET=$?

cmp_files "file1"
CMP_FILE1=$?

cmp_files "file2"
CMP_FILE2=$?

variables_must_eq RET 2 \
	CMP_FILE1 0 \
	CMP_FILE2 0
