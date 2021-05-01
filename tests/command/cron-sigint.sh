. "$(dirname "$0")"/../util/common.sh

run_test "-q" ./util/test-kill-pid $SIGINT
RET=$?

eval $(./util/test-waitpid ./dtee ./dtee "-q" ./util/test-kill-pid $SIGINT)
RET2=$?

variables_must_eq RET $((128 + $SIGINT)) \
	RET2 0 \
	WIFSIGNALED 1 \
	WTERMSIG $SIGINT
