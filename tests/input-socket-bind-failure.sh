#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD="./libsocket-bind-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
