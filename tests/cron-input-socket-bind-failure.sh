. "$(dirname "$0")"/util/common.sh

# If the input socket bind fails, execute the command without dtee processing
TEST_LD_PRELOAD=(./libtest-socket-bind-failure ./libtest-fake-getpid ./libtest-fake-getuid)
run_test "-q" "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
