#!/bin/bash
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.err-append.txt"
}

run_test -e "$TESTDIR/$NAME.file.err-append.txt" "$RUN"
RET=$?

cmp_files "file.err-append"
CMP_ERR_A=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_ERR_A 0
