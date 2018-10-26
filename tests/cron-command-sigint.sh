. "$(dirname "$0")"/util/common.sh

run_test "-q" ./test-kill-pid $SIGINT
RET=$?

eval $(./test-waitpid ./dtee ./dtee "-q" ./test-kill-pid $SIGINT)
RET2=$?

variables_must_eq RET $((128 + $SIGINT)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGINT
