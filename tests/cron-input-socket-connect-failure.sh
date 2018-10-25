. "$(dirname "$0")"/util/common.sh

# If the input socket connect fails, execute the command without dtee processing
TEST_LD_PRELOAD="./libtest-socket-connect-failure.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
run_test "-q" "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
