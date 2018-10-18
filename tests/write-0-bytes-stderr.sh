#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.err-overwrite.txt"
}

run_test -E "$TESTDIR/$NAME.file.err-overwrite.txt" ./test-writes-of-n-bytes STDERR_FILENO 0 0
RET=$?

cmp_files "${0/.sh/.file.err-overwrite.txt}" "$TESTDIR/$NAME.file.err-overwrite.txt"
CMP_ERR_O=$?

variables_must_eq RET 0 \
	CMP_ERR_O 0
