#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-append.txt" "$TESTDIR/$NAME.file.err-append.txt" "$TESTDIR/$NAME.file.com-append.txt"
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt" "$TESTDIR/$NAME.file.err-overwrite.txt" "$TESTDIR/$NAME.file.com-overwrite.txt"
}

COMMON_TEST_LD_PRELOAD=("${COMMON_TEST_LD_PRELOAD[@]/.\/libexecvp-fd-check.so}")
TEST_NO_STDIN=1

TEST_LD_PRELOAD="./libmkdtemp-consistent.so:./libfake-getpid.so:./libfake-getuid.so"
run_test \
	-o "$TESTDIR/$NAME.file.out-append.txt" -O "$TESTDIR/$NAME.file.out-overwrite.txt" \
	-e "$TESTDIR/$NAME.file.err-append.txt" -E "$TESTDIR/$NAME.file.err-overwrite.txt" \
	-c "$TESTDIR/$NAME.file.com-append.txt" -C "$TESTDIR/$NAME.file.com-overwrite.txt" \
	-q ./test-report 'Hello World!' ETAOIN SHRDLU
RET=$?

cmp_files "${0/.sh/.file.out-append.txt}" "$TESTDIR/$NAME.file.out-append.txt"
CMP_OUT_A=$?

cmp_files "${0/.sh/.file.out-overwrite.txt}" "$TESTDIR/$NAME.file.out-overwrite.txt"
CMP_OUT_O=$?

cmp_files "${0/.sh/.file.err-append.txt}" "$TESTDIR/$NAME.file.err-append.txt"
CMP_ERR_A=$?

cmp_files "${0/.sh/.file.err-overwrite.txt}" "$TESTDIR/$NAME.file.err-overwrite.txt"
CMP_ERR_O=$?

cmp_files "${0/.sh/.file.com-append.txt}" "$TESTDIR/$NAME.file.com-append.txt"
CMP_COM_A=$?

cmp_files "${0/.sh/.file.com-overwrite.txt}" "$TESTDIR/$NAME.file.com-overwrite.txt"
CMP_COM_O=$?

echo RET $RET
echo CMP_OUT_A $CMP_OUT_A
echo CMP_OUT_O $CMP_OUT_O
echo CMP_ERR_A $CMP_ERR_A
echo CMP_ERR_O $CMP_ERR_O
echo CMP_COM_A $CMP_COM_A
echo CMP_COM_O $CMP_COM_O
if [ $RET -eq 0 ] && [ $CMP_OUT_A -eq 0 ] && [ $CMP_OUT_O -eq 0 ] && [ $CMP_ERR_A -eq 0 ] && [ $CMP_ERR_O -eq 0 ] && [ $CMP_COM_A -eq 0 ] && [ $CMP_COM_O -eq 0 ]; then
	exit 0
fi
exit 1
