#!/bin/bash
. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD="./libsocket-shutdown-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
