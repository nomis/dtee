#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f "$TESTDIR/$NAME.file.out-append.txt" "$TESTDIR/$NAME.file.err-append.txt" "$TESTDIR/$NAME.file.com-append.txt"
rm -f "$TESTDIR/$NAME.file.out-overwrite.txt" "$TESTDIR/$NAME.file.err-overwrite.txt" "$TESTDIR/$NAME.file.com-overwrite.txt"

COMMON_TEST_LD_PRELOAD=("${COMMON_TEST_LD_PRELOAD[@]/.\/libexecvp-fd-check.so}")

echo -n "I" | run_test_once \
	-o "$TESTDIR/$NAME.file.out-append.txt" -O "$TESTDIR/$NAME.file.out-overwrite.txt" \
	-e "$TESTDIR/$NAME.file.err-append.txt" -E "$TESTDIR/$NAME.file.err-overwrite.txt" \
	-c "$TESTDIR/$NAME.file.com-append.txt" -C "$TESTDIR/$NAME.file.com-overwrite.txt" \
	./test-report 'Hello World!' ETAOIN SHRDLU 42</dev/null
RET=$?

cmp "$TESTDIR/$NAME.file.out-append.txt" "${0/.sh/.file.out-append.txt}"
CMP_OUT_A=$?
[ $CMP_OUT_A -ne 0 ] && diff -U4 "${0/.sh/.file.out-append.txt}" "$TESTDIR/$NAME.file.out-append.txt"

cmp "$TESTDIR/$NAME.file.out-overwrite.txt" "${0/.sh/.file.out-overwrite.txt}"
CMP_OUT_O=$?
[ $CMP_OUT_O -ne 0 ] && diff -U4 "${0/.sh/.file.out-overwrite.txt}" "$TESTDIR/$NAME.file.out-overwrite.txt"

cmp "$TESTDIR/$NAME.file.err-append.txt" "${0/.sh/.file.err-append.txt}"
CMP_ERR_A=$?
[ $CMP_ERR_A -ne 0 ] && diff -U4 "${0/.sh/.file.err-append.txt}" "$TESTDIR/$NAME.file.err-append.txt"

cmp "$TESTDIR/$NAME.file.err-overwrite.txt" "${0/.sh/.file.err-overwrite.txt}"
CMP_ERR_O=$?
[ $CMP_ERR_O -ne 0 ] && diff -U4 "${0/.sh/.file.err-overwrite.txt}" "$TESTDIR/$NAME.file.err-overwrite.txt"

cmp "$TESTDIR/$NAME.file.com-append.txt" "${0/.sh/.file.com-append.txt}"
CMP_COM_A=$?
[ $CMP_COM_A -ne 0 ] && diff -U4 "${0/.sh/.file.com-append.txt}" "$TESTDIR/$NAME.file.com-append.txt"

cmp "$TESTDIR/$NAME.file.com-overwrite.txt" "${0/.sh/.file.com-overwrite.txt}"
CMP_COM_O=$?
[ $CMP_COM_O -ne 0 ] && diff -U4 "${0/.sh/.file.com-overwrite.txt}" "$TESTDIR/$NAME.file.com-overwrite.txt"

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
