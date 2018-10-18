#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt"
}

run_test -O "$TESTDIR/$NAME.file.out-overwrite.txt" ./test-writes-of-n-bytes STDOUT_FILENO PIPE_BUF 0
RET=$?

cmp_files "${0/.sh/.file.out-overwrite.txt}" "$TESTDIR/$NAME.file.out-overwrite.txt"
CMP_OUT_O=$?

variables_must_eq RET 0 \
	CMP_OUT_O 0
