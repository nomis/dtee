#!/bin/bash
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.err-overwrite.txt"
}

# Writes of 0 bytes should work and not be considered EOF
run_test -q -E "$TESTDIR/$NAME.file.err-overwrite.txt" ./test-write-0-bytes STDERR_FILENO 0
RET=$?

cmp_files "file.err-overwrite"
CMP_ERR_O=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_ERR_O 0
