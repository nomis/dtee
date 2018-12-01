. "$(dirname "$0")"/util/common.sh

# Allow the first few lines to work
export DTEE_TEST_SOCKET_RECV_FAILURE_ALLOW=4

TEST_LD_PRELOAD=(test-socket-recv-failure)
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_IOERR
