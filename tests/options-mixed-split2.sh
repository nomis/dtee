#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# End of options signifier stops all following options parsing, even if the next word is an option
run_test "--debug-options" "-o" "filename1" "-e" "filename2" "--" "-c" "filename3" "command"
RET=$?

variables_must_eq RET 0
