. "$(dirname "$0")"/util/common.sh

# Windows Subsystem for Linux (4.4.0-43) always claims to have written a core dump on SIGQUIT
[[ $(uname -r) =~ Microsoft ]] && exit $TEST_EX_SKIP

# Increase core size to unlimited and then the command will disable core dumps
ulimit -H -c unlimited || exit $TEST_EX_SKIP
ulimit -S -c unlimited || exit $TEST_EX_SKIP
export DTEE_TEST_RLIMIT_CORE=0

run_test "-q" ./util/test-kill-pid $SIGQUIT
RET=$?

eval $(./util/test-waitpid ./dtee ./dtee "-q" ./util/test-kill-pid $SIGQUIT)
RET2=$?

variables_must_eq RET $((128 + $SIGQUIT)) \
	RET2 0 \
	WIFSIGNALED 0
