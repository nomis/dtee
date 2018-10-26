. "$(dirname "$0")"/util/common.sh

# Increase core size to unlimited and then the command will disable core dumps
ulimit -H -c unlimited || exit $TEST_EX_SKIP
ulimit -S -c unlimited || exit $TEST_EX_SKIP
export DTEE_TEST_RLIMIT_CORE=0

run_test "-q" ./test-kill-pid 3
RET=$?

eval $(./test-waitpid ./dtee ./dtee "-q" ./test-kill-pid 3)
RET2=$?

variables_must_eq RET $((128 + $SIGQUIT)) \
	RET2 0 \
	WIFSIGNALED 0
