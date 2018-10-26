. "$(dirname "$0")"/util/common.sh

run_test ./test-kill-pid $SIGINT
RET=$?

eval $(./test-waitpid ./dtee ./dtee ./test-kill-pid $SIGINT)
RET2=$?

variables_must_eq RET $((128 + $SIGINT)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGINT
