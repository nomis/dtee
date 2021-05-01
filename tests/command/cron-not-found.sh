. "$(dirname "$0")"/../util/common.sh

run_test "-q" "./command_not_found"
RET=$?

variables_must_eq RET $EX_NOINPUT
