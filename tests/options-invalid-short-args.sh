#!/bin/bash
. "$(dirname "$0")"/util/common.sh

run_test "--debug-options" "-a" "your" "-b" "are" "-b" "to" "us"
RET=$?

variables_must_eq RET $EX_USAGE
