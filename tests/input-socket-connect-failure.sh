#!/bin/bash
. "$(dirname "$0")"/util/common.sh
set -x

TEST_LD_PRELOAD="./libsocket-connect-failure.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "$RUN"
RET=$?

set +x
variables_must_eq RET $EX_UNAVAILABLE
