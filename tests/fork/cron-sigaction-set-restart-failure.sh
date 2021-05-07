. "$(dirname "$0")"/../util/common.sh

TEST_LD_PRELOAD=(test-sigaction-set-restart-failure test-fake-getpid test-fake-getuid)
run_test "-q" "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
