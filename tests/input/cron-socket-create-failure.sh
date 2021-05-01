. "$(dirname "$0")"/../util/common.sh

# If the input socket cannot be created, execute the command without dtee processing
TEST_LD_PRELOAD=(test-socket-unix-failure test-fake-getpid test-fake-getuid)
run_test "-q" "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
