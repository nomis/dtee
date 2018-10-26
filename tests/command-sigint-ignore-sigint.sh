. "$(dirname "$0")"/util/common.sh

# Ignoring SIGINT does not affect behaviour when the child process receives SIGINT
run_test -i ./test-kill-pid $SIGINT
RET=$?

eval $(./test-waitpid ./dtee ./dtee -i ./test-kill-pid $SIGINT)
RET2=$?

variables_must_eq RET $((128 + $SIGINT)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGINT
