. "$(dirname "$0")"/util/common.sh

run_test "-q" ./test-kill-pid 15
RET=$?

eval $(./test-waitpid ./dtee ./dtee "-q" ./test-kill-pid 15)
RET2=$?

variables_must_eq RET $((128 + 15)) \
	RET2 0 \
	WIFSIGNALED 0
