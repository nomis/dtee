. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-overwrite.txt"
}

# Cause socket close to fail in the parent process
export DTEE_TEST_SOCKET_CLOSE_FAILURE_PARENT=1

no_ld_preload "./libtest-execvp-fd-check.so"

TEST_LD_PRELOAD="./libtest-socket-close-failure.so"
run_test -q -C "$TESTDIR/$NAME.file.com-overwrite.txt" "$RUN"
RET=$?

cmp_files "file.com-overwrite"
CMP_COM_O=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_COM_O 0
