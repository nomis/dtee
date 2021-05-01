. "$(dirname "$0")"/../util/common.sh

run_test "-q" "$RUN"
RET=$?

variables_must_eq RET 42
