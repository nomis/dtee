. "$(dirname -- "$0")"/../util/common.sh

run_test "-q" ./util/test-kill-pid $SIGKILL
RET=$?

eval_ret ./util/test-waitpid ./dtee ./dtee "-q" ./util/test-kill-pid $SIGKILL
RET2=$?

variables_must_eq RET $((128 + $SIGKILL)) \
	RET2 0 \
	WIFSIGNALED 0
