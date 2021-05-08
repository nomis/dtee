. "$(dirname -- "$0")"/../util/common.sh

# Empty command stops all following options parsing
run_test "--debug-options" "" "-o" "filename1" "-e" "filename2"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
