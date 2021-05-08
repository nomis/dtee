. "$(dirname -- "$0")"/../util/common.sh

# Ignoring SIGINT does not affect behaviour when the child process receives SIGINT
run_test -i ./util/test-kill-pid $SIGINT
RET=$?

eval_ret ./util/test-waitpid ./dtee ./dtee -i ./util/test-kill-pid $SIGINT
RET2=$?

variables_must_eq RET $((128 + $SIGINT)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGINT
