#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f "$TESTDIR/$NAME.file.out-append" "$TESTDIR/$NAME.file.err-append" "$TESTDIR/$NAME.file.com-append"
rm -f "$TESTDIR/$NAME.file.out-overwrite" "$TESTDIR/$NAME.file.err-overwrite" "$TESTDIR/$NAME.file.com-overwrite"

echo -n "I" | run_test_once \
	-o "$TESTDIR/$NAME.file.out-append" -O "$TESTDIR/$NAME.file.out-overwrite" \
	-e "$TESTDIR/$NAME.file.err-append" -E "$TESTDIR/$NAME.file.err-overwrite" \
	-c "$TESTDIR/$NAME.file.com-append" -C "$TESTDIR/$NAME.file.com-overwrite" \
	-q ./test-report 'Hello World!' ETAOIN SHRDLU
RET=$?

cmp "$TESTDIR/$NAME.file.out-append" "${0/.sh/.file.out-append}"
CMP_OUT_A=$?
[ $CMP_OUT_A -ne 0 ] && diff -U4 "${0/.sh/.file.out-append}" "$TESTDIR/$NAME.file.out-append"

cmp "$TESTDIR/$NAME.file.out-overwrite" "${0/.sh/.file.out-overwrite}"
CMP_OUT_O=$?
[ $CMP_OUT_O -ne 0 ] && diff -U4 "${0/.sh/.file.out-overwrite}" "$TESTDIR/$NAME.file.out-overwrite"

cmp "$TESTDIR/$NAME.file.err-append" "${0/.sh/.file.err-append}"
CMP_ERR_A=$?
[ $CMP_ERR_A -ne 0 ] && diff -U4 "${0/.sh/.file.err-append}" "$TESTDIR/$NAME.file.err-append"

cmp "$TESTDIR/$NAME.file.err-overwrite" "${0/.sh/.file.err-overwrite}"
CMP_ERR_O=$?
[ $CMP_ERR_O -ne 0 ] && diff -U4 "${0/.sh/.file.err-overwrite}" "$TESTDIR/$NAME.file.err-overwrite"

cmp "$TESTDIR/$NAME.file.com-append" "${0/.sh/.file.com-append}"
CMP_COM_A=$?
[ $CMP_COM_A -ne 0 ] && diff -U4 "${0/.sh/.file.com-append}" "$TESTDIR/$NAME.file.com-append"

cmp "$TESTDIR/$NAME.file.com-overwrite" "${0/.sh/.file.com-overwrite}"
CMP_COM_O=$?
[ $CMP_COM_O -ne 0 ] && diff -U4 "${0/.sh/.file.com-overwrite}" "$TESTDIR/$NAME.file.com-overwrite"

echo RET $RET
echo CMP_OUT_A $CMP_OUT_A
echo CMP_OUT_A $CMP_OUT_O
echo CMP_ERR_A $CMP_ERR_A
echo CMP_ERR_A $CMP_ERR_O
echo CMP_COM_A $CMP_COM_A
echo CMP_COM_A $CMP_COM_O
if [ $RET -eq 0 ] && [ $CMP_OUT_A -eq 0 ] && [ $CMP_OUT_O -eq 0 ] && [ $CMP_ERR_A -eq 0 ] && [ $CMP_ERR_O -eq 0 ] && [ $CMP_COM_A -eq 0 ] && [ $CMP_COM_O -eq 0 ]; then
	exit 0
fi
exit 1
