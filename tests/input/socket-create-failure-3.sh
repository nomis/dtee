. "$(dirname -- "$0")"/../util/common.sh

# Allow the first two sockets to work
export DTEE_TEST_SOCKET_UNIX_FAILURE_ALLOW=2

TEST_LD_PRELOAD=(test-socket-unix-failure test-fake-getpid test-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
