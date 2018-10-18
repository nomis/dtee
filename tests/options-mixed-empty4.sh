#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Empty command stops all following options parsing, even if they're empty
run_test "--debug-options" "" "-o" "" "-e" "filename"
RET=$?

variables_must_eq RET 0
