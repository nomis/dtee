. "$(dirname -- "$0")"/../util/common.sh

export DTEE_TEST_FCNTL_NON_BLOCKING_STDOUT=0
export DTEE_TEST_FCNTL_NON_BLOCKING_STDERR=1
export DTEE_TEST_FCNTL_GETFL_FAILURE_FD=
export DTEE_TEST_FCNTL_SETFL_FAILURE_FD=2

no_ld_preload "test-execvp-fd-check"

TEST_LD_PRELOAD=(test-fcntl-flags)
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_CANTCREAT
