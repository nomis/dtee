. "$(dirname "$0")"/util/common.sh

no_ld_preload "./libtest-execvp-fd-check"

TEST_LD_PRELOAD=(./libtest-socket-add-extra-source ./libtest-mkdtemp-consistent ./libtest-fake-getpid ./libtest-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
