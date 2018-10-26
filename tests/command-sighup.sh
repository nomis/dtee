. "$(dirname "$0")"/util/common.sh

run_test ./test-kill-pid $SIGHUP
RET=$?

eval $(./test-waitpid ./dtee ./dtee ./test-kill-pid $SIGHUP)
RET2=$?

variables_must_eq RET $((128 + $SIGHUP)) \
	RET2 0 \
	WIFSIGNALED 0
