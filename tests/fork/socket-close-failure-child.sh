. "$(dirname "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-overwrite.txt"
}

# Cause socket close to fail in the child process
export DTEE_TEST_SOCKET_CLOSE_FAILURE_CHILD=1

no_ld_preload "test-execvp-fd-check"

TEST_LD_PRELOAD=(test-socket-close-failure)
run_test -C "$TESTDIR/$NAME.file.com-overwrite.txt" "$RUN"
RET=$?

cmp_files "file.com-overwrite"
CMP_COM_O=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_COM_O 0
