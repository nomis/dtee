. "$(dirname "$0")"/util/common.sh

# Increase core size to unlimited and then the command will set it again
ulimit -H -c unlimited || exit $TEST_EX_SKIP
ulimit -S -c unlimited || exit $TEST_EX_SKIP
export DTEE_TEST_RLIMIT_CORE=RLIM_INFINITY

run_test "-q" ./test-kill-pid $SIGQUIT
RET=$?

eval $(./test-waitpid ./dtee ./dtee "-q" ./test-kill-pid $SIGQUIT)
RET2=$?

variables_must_eq RET $((128 + $SIGQUIT)) \
	RET2 0 \
	WIFSIGNALED 0
