. "$(dirname "$0")"/util/common.sh

run_test ./test-kill-pid $SIGTERM
RET=$?

eval $(./test-waitpid ./dtee ./dtee ./test-kill-pid $SIGTERM)
RET2=$?

variables_must_eq RET $((128 + $SIGTERM)) \
	RET2 0 \
	WIFSIGNALED 0
