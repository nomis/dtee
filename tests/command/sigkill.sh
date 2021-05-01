. "$(dirname "$0")"/../util/common.sh

run_test ./util/test-kill-pid $SIGKILL
RET=$?

eval $(./util/test-waitpid ./dtee ./dtee ./util/test-kill-pid $SIGKILL)
RET2=$?

variables_must_eq RET $((128 + $SIGKILL)) \
	RET2 0 \
	WIFSIGNALED 0
