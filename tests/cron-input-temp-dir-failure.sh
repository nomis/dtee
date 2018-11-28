. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD=(./libtest-mkdtemp-failure ./libtest-fake-getpid ./libtest-fake-getuid)
run_test -q "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
