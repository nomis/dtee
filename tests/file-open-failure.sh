#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

(umask 0333; touch file_not_writeable)
run_test "-o" "file_not_writeable" "$RUN"
RET=$?

variables_must_eq RET $EX_CANTCREAT
