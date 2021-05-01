. "$(dirname "$0")"/../util/common.sh

# End of options signifier stops all following options parsing
run_test "--debug-options" "-o" "filename1" "-e" "filename2" "--" "command" "-c" "filename3" "another argument"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
