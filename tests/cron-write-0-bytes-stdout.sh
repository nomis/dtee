#!/bin/bash
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt"
}

# Writes of 0 bytes should work and not be considered EOF
run_test -q -O "$TESTDIR/$NAME.file.out-overwrite.txt" ./test-write-0-bytes STDOUT_FILENO 1
RET=$?

cmp_files "${0/.sh/.file.out-overwrite.txt}" "$TESTDIR/$NAME.file.out-overwrite.txt"
CMP_OUT_O=$?

variables_must_eq RET $EXIT_FAILURE \
	CMP_OUT_O 0
