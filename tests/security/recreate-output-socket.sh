. "$(dirname "$0")"/../util/common.sh

no_ld_preload "test-execvp-fd-check"
TEST_EXTRA_OUTPUT=1

TEST_LD_PRELOAD=(test-security-recreate-output-socket test-mkdtemp-consistent test-fake-getpid test-fake-getuid)
# It should not be possible to recreate an output socket and connect it to the
# input socket, even after the child process exits
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS

