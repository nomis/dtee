. "$(dirname -- "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt" "$TESTDIR/$NAME.file.com-overwrite.txt"
}

TEST_LD_PRELOAD=(test-write-truncate-cron test-fake-getpid test-fake-getuid)
run_test \
	-O "$TESTDIR/$NAME.file.out-overwrite.txt" \
	-C "$TESTDIR/$NAME.file.com-overwrite.txt" \
	-q "$RUN"
RET=$?

cmp_files "file.out-overwrite"
CMP_OUT_O=$?

cmp_files "file.com-overwrite"
CMP_COM_O=$?

variables_must_eq RET $EX_IOERR \
	CMP_OUT_O 0 \
	CMP_COM_O 0
