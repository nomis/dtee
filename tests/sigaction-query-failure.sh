. "$(dirname "$0")"/util/common.sh

no_ld_preload "test-fork-sigchld-check"

TEST_LD_PRELOAD=(test-sigaction-query-failure test-fake-getpid test-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_IOERR
