#!/bin/bash
. "$(dirname "$0")"/util/common.sh

run_test "--debug-options" "command" "argument 1"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
