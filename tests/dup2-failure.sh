#!/bin/bash
. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-overwrite.txt"
}

TEST_LD_PRELOAD="./libtest-dup2-failure.so"
# There will be no output received by dtee if dup2 fails
run_test -C "$TESTDIR/$NAME.file.com-overwrite.txt" "$RUN"
RET=$?

cmp_files "${0/.sh/.file.com-overwrite.txt}" "$TESTDIR/$NAME.file.com-overwrite.txt"
CMP_COM_O=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_COM_O 0
