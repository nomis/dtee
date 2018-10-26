. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD="./libtest-kill-failure.so"
# If kill fails, we can only exit with a simulated SIGINT
run_test ./test-kill-pid 2
RET=$?

eval $(LD_PRELOAD="$TEST_LD_PRELOAD" ./test-waitpid ./dtee ./dtee ./test-kill-pid 2)
RET2=$?

variables_must_eq RET $((128 + $SIGINT)) \
	RET2 0 \
	WIFSIGNALED 0
