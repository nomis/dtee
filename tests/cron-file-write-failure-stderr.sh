#!/bin/bash
. "$(dirname "$0")"/util/common.sh

run_test "-q" "-e" "/dev/full" "$RUN"
RET=$?

variables_must_eq RET $EX_IOERR
