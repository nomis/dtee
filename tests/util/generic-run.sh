. "$(dirname "$0")"/util/common.sh

run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
