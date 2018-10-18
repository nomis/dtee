#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.err-append"
}

run_test -e "$TESTDIR/$NAME.file.err-append" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.err-append}" "$TESTDIR/$NAME.file.err-append"
CMP_ERR_A=$?

variables_must_eq RET 0 \
	CMP_ERR_A 0
