. "$(dirname "$0")"/util/common.sh

# Empty filename followed by empty command
run_test "--debug-options" "-o" "" ""
RET=$?

variables_must_eq RET $EXIT_SUCCESS
