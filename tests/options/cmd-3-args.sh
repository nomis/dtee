. "$(dirname "$0")"/../util/common.sh

# Numbers are out of order to check they're not being sorted
run_test "--debug-options" "command" "argument 3" "argument 1" "argument 2"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
