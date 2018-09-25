#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

rm -f "$TESTDIR/$NAME.file.com-append"

run_test_once -c "$TESTDIR/$NAME.file.com-append" "$RUN"
RET=$?

cmp "$TESTDIR/$NAME.file.com-append" "${0/.sh/.file.com-append}"
CMP_COM_A=$?
[ $CMP_COM_A -ne 0 ] && diff -U4 "${0/.sh/.file.com-append}" "$TESTDIR/$NAME.file.com-append"

echo RET $RET
echo CMP_COM_A $CMP_COM_A
if [ $RET -eq 0 ] && [ $CMP_COM_A -eq 0 ]; then
	exit 0
fi
exit 1
