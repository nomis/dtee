. "$(dirname -- "$0")"/../util/common.sh

export DTEE_TEST_FCNTL_NON_BLOCKING_STDOUT=1
export DTEE_TEST_FCNTL_NON_BLOCKING_STDERR=0
export DTEE_TEST_FCNTL_GETFL_FAILURE_FD=
export DTEE_TEST_FCNTL_SETFL_FAILURE_FD=

no_ld_preload "test-execvp-fd-check"

TEST_LD_PRELOAD=(test-fcntl-flags)
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
