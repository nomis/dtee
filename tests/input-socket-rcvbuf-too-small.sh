. "$(dirname "$0")"/util/common.sh

if [ "$UNAME" == "GNU" ]; then
	exit $TEST_EX_SKIP
fi

export DTEE_TEST_GETSOCKOPT_RCVBUF="max(PIPE_BUF,BUFSIZ)/4"

# If the outgoing SO_RCVBUF is smaller than max(PIPE_BUF,BUFSIZ), it should be raised to at least max(PIPE_BUF,BUFSIZ)
# The incoming SO_SNDBUF will be set to the same as the observed SO_RCVBUF
TEST_LD_PRELOAD=(./libtest-socket-setsockopt-monitor ./libtest-socket-getsockopt-fake-rcvbuf ./libtest-mkdtemp-consistent ./libtest-fake-getpid ./libtest-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
