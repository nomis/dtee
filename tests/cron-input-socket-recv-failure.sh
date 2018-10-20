#!/bin/bash
. "$(dirname "$0")"/util/common.sh

# Allow the first few lines to work
export DTEE_TEST_SOCKET_RECV_FAILURE_ALLOW=3

TEST_LD_PRELOAD="./libtest-socket-recv-failure.so"
run_test -q ./test-lorem-ipsum-late-exit
RET=$?

variables_must_eq RET $EX_IOERR
