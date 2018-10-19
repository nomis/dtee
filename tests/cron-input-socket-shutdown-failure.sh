#!/bin/bash
. "$(dirname "$0")"/util/common.sh

# If the input socket shutdown fails, execute the command without dtee processing
TEST_LD_PRELOAD="./libsocket-shutdown-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "-q" "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
