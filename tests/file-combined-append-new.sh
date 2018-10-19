#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-append.txt"
}

run_test -c "$TESTDIR/$NAME.file.com-append.txt" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.com-append.txt}" "$TESTDIR/$NAME.file.com-append.txt"
CMP_COM_A=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_COM_A 0
