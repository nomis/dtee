#!/bin/bash
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.err-append"
	echo Existing line 1 >"$TESTDIR/$NAME.file.err-append"
	echo Existing line 2 >>"$TESTDIR/$NAME.file.err-append"
	echo Existing line 3 >>"$TESTDIR/$NAME.file.err-append"
}

run_test -e "$TESTDIR/$NAME.file.err-append" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.err-append}" "$TESTDIR/$NAME.file.err-append"
CMP_ERR_A=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_ERR_A 0
