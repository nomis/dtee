. "$(dirname "$0")"/util/common.sh

run_test "--debug-options" "command"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
