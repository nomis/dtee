. "$(dirname "$0")"/util/common.sh

no_ld_preload "test-execvp-fd-check"

TEST_LD_PRELOAD=(test-socket-add-extra-source test-mkdtemp-consistent test-fake-getpid test-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
