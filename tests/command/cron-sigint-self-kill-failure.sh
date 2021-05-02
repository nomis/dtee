. "$(dirname "$0")"/../util/common.sh

TEST_LD_PRELOAD=(test-kill-failure)
# If kill fails, we can only exit with a simulated SIGINT
run_test "-q" ./util/test-kill-pid $SIGINT
RET=$?

eval_ret run_with_preload ./util/test-waitpid ./dtee ./dtee "-q" ./util/test-kill-pid $SIGINT
RET2=$?

variables_must_eq RET $((128 + $SIGINT)) \
	RET2 0 \
	WIFSIGNALED 0
