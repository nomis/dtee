#!/bin/bash
. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD="./libmkdtemp-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test -q "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
