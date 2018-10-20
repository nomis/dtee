#!/bin/bash
. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD="./libtest-mkdtemp-failure.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
run_test -q "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
