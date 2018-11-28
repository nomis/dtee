. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD=(./libtest-socket-unix-failure ./libtest-fake-getpid ./libtest-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
