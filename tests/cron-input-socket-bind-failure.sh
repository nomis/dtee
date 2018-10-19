#!/bin/bash
. "$(dirname "$0")"/util/common.sh

# If the input socket bind fails, execute the command without dtee processing
TEST_LD_PRELOAD="./libsocket-bind-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "-q" "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
