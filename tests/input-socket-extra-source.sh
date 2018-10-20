#!/bin/bash
. "$(dirname "$0")"/util/common.sh

COMMON_TEST_LD_PRELOAD=("${COMMON_TEST_LD_PRELOAD[@]/.\/libtest-execvp-fd-check.so}")

TEST_LD_PRELOAD="./libtest-socket-add-extra-source.so:./libtest-mkdtemp-consistent.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
