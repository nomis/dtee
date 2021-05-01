. "$(dirname "$0")"/../util/common.sh

run_test "-q" ./util/test-kill-pid $SIGTERM
RET=$?

eval $(./util/test-waitpid ./dtee ./dtee "-q" ./util/test-kill-pid $SIGTERM)
RET2=$?

variables_must_eq RET $((128 + $SIGTERM)) \
	RET2 0 \
	WIFSIGNALED 0
