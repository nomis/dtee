#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.err-overwrite"
}

run_test -E "$TESTDIR/$NAME.file.err-overwrite" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.err-overwrite}" "$TESTDIR/$NAME.file.err-overwrite"
CMP_ERR_O=$?

variables_must_eq RET 0 \
	CMP_ERR_O 0
