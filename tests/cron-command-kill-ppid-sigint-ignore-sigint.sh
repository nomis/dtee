#!/bin/bash
. "$(dirname "$0")"/util/common.sh

run_test -q -i "$RUN"
RET=$?

variables_must_eq RET 0
