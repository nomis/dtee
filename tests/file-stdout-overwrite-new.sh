#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt"
}

run_test -O "$TESTDIR/$NAME.file.out-overwrite.txt" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.out-overwrite.txt}" "$TESTDIR/$NAME.file.out-overwrite.txt"
CMP_OUT_O=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_OUT_O 0
