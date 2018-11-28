. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-overwrite.txt"
}

TEST_LD_PRELOAD=(./libtest-fork-failure)
# There will be no output received by dtee if fork fails but the command will be executed directly
run_test -q -C "$TESTDIR/$NAME.file.com-overwrite.txt" "$RUN"
RET=$?

cmp_files "file.com-overwrite"
CMP_COM_O=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_COM_O 0
