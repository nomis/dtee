#!/bin/bash
. "$(dirname "$0")"/util/common.sh

run_test "--debug-options" "-a" "-y" "-b" "-a" "-b" "-t" "-u"
RET=$?

variables_must_eq RET $EX_USAGE
