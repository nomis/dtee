. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-overwrite.txt"
}

TEST_LD_PRELOAD="./libtest-fork-failure.so"
# If fork fails then dtee will exit without running the command
run_test -C "$TESTDIR/$NAME.file.com-overwrite.txt" "$RUN"
RET=$?

cmp_files "file.com-overwrite"
CMP_COM_O=$?

variables_must_eq RET $EX_OSERR \
	CMP_COM_O 0
