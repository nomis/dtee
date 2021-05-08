. "$(dirname -- "$0")"/../util/common.sh

# End of options signifier stops all following options parsing
run_test "--debug-options" "--" "command" "-o" "filename1" "-e" "filename2" "-c" "filename3" "another argument"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
