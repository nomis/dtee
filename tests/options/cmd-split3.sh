. "$(dirname -- "$0")"/../util/common.sh

# End of options signifier stops all following options parsing, even with empty command and empty argument
run_test "--debug-options" "--" "" ""
RET=$?

variables_must_eq RET $EXIT_SUCCESS
