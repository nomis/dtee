. "$(dirname "$0")"/util/common.sh

no_ld_preload "./libtest-execvp-fd-check"
TEST_EXTRA_OUTPUT=1

TEST_LD_PRELOAD=(./libtest-security-recreate-output-socket ./libtest-mkdtemp-consistent ./libtest-fake-getpid ./libtest-fake-getuid)
# It should not be possible to recreate an output socket and connect it to the
# input socket, even after the child process exits
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS

