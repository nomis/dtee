. "$(dirname "$0")"/util/common.sh

TEST_EXEC=./cronty
run_test "$RUN"
RET=$?

variables_must_eq RET 42
