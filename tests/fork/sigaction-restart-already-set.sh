. "$(dirname -- "$0")"/../util/common.sh

TEST_LD_PRELOAD=(test-sigaction-restart-already-set test-fake-getpid test-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
