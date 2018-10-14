#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-overwrite.txt"
}

# Cause socket close to fail in the parent process
export DTEE_TEST_SOCKET_CLOSE_FAILURE_PARENT=1

COMMON_TEST_LD_PRELOAD=("${COMMON_TEST_LD_PRELOAD[@]/.\/libexecvp-fd-check.so}")

TEST_LD_PRELOAD="./libsocket-close-failure.so"
run_test -C "$TESTDIR/$NAME.file.com-overwrite.txt" "$RUN"
RET=$?

cmp "$TESTDIR/$NAME.file.com-overwrite.txt" "${0/.sh/.file.com-overwrite.txt}"
CMP_COM_O=$?
[ $CMP_COM_O -ne 0 ] && diff -U4 "${0/.sh/.file.com-overwrite.txt}" "$TESTDIR/$NAME.file.com-overwrite.txt"

echo RET $RET
echo CMP_COM_O $CMP_COM_O
if [ $RET -eq 0 ] && [ $CMP_COM_O -eq 0 ]; then
	exit 0
fi
exit 1
