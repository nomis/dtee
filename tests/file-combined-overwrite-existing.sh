#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-overwrite.txt"
	echo Existing line >"$TESTDIR/$NAME.file.com-overwrite.txt"
}

run_test -C "$TESTDIR/$NAME.file.com-overwrite.txt" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.com-overwrite.txt}" "$TESTDIR/$NAME.file.com-overwrite.txt"
CMP_COM_O=$?

variables_must_eq RET 0 \
	CMP_COM_O 0
