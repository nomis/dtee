#!/bin/bash
. "$(dirname "$0")"/util/common.sh

# Check that obscure Boost syntax "--=..." works as expected 😩
run_test "--debug-options" "-o" "stdout_append" "--=command" "-e" "stderr_append" "--=argument 1" "-c" "combined_append" "--=argument 2" "-q" "--=argument 3"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
