. "$(dirname -- "$0")"/../util/common.sh

export DTEE_TEST_FCNTL_GETFD_FAILURE_FD=1

TEST_LD_PRELOAD=(test-fcntl-failures-flags)

# Test with fcntl() failure (other than EBADF)
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_OSERR
