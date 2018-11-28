. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD=(./libtest-kill-failure)
# If kill fails, we can only exit with a simulated SIGINT
run_test ./test-kill-pid $SIGINT
RET=$?

eval $(run_with_preload ./test-waitpid ./dtee ./dtee ./test-kill-pid $SIGINT)
RET2=$?

variables_must_eq RET $((128 + $SIGINT)) \
	RET2 0 \
	WIFSIGNALED 0
