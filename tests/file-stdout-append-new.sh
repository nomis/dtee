#!/bin/bash
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-append.txt"
}

run_test -o "$TESTDIR/$NAME.file.out-append.txt" "$RUN"
RET=$?

cmp_files "file.out-append"
CMP_OUT_A=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_OUT_A 0
