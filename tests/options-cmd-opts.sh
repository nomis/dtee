#!/bin/bash
. "$(dirname "$0")"/util/common.sh

# Command stops further options parsing
run_test "--debug-options" "command" "-o" "argument 1" "-x" "argument 2" "-y" "argument 3" "--help"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
