#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# If the temporary output file cannot be read, it should output an error message
TEST_LD_PRELOAD="./libmkostemp-writeonly.so:./libfake-getpid.so:./libfake-getuid.so"
run_test "-q" "$RUN"
RET=$?

# Output errors prevent a 0 exit status
variables_must_eq RET $EX_IOERR
