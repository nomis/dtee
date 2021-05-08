. "$(dirname -- "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-overwrite.txt"
}

# Cause socket close to fail in the parent process
export DTEE_TEST_SOCKET_CLOSE_FAILURE_PARENT=1

no_ld_preload "test-execvp-fd-check"

TEST_LD_PRELOAD=(test-socket-close-failure)
run_test -C "$TESTDIR/$NAME.file.com-overwrite.txt" "$RUN"
RET=$?

cmp_files "file.com-overwrite"
CMP_COM_O=$?

variables_must_eq RET $EX_IOERR \
	CMP_COM_O 0
