. "$(dirname "$0")"/../util/common.sh

# Allow the first socket to work
export DTEE_TEST_SOCKET_SHUTDOWN_FAILURE_ALLOW=1

TEST_LD_PRELOAD=(test-socket-shutdown-failure test-fake-getpid test-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
