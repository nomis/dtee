#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test "--debug-options" "--all=your" "--base=are" "--belong=to" "--us="
RET=$?

variables_must_eq RET $EX_USAGE
