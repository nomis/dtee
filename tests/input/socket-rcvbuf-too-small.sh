. "$(dirname -- "$0")"/../util/common.sh

if [ "$UNAME" == "GNU" ]; then
	exit $TEST_EX_SKIP
fi

export DTEE_TEST_GETSOCKOPT_RCVBUF="max(PIPE_BUF,BUFSIZ)/4"

# If the outgoing SO_RCVBUF is smaller than max(PIPE_BUF,BUFSIZ), it should be raised to at least max(PIPE_BUF,BUFSIZ)
# The incoming SO_SNDBUF will be set to the same as the observed SO_RCVBUF
TEST_LD_PRELOAD=(test-socket-setsockopt-monitor test-socket-getsockopt-fake-rcvbuf test-mkdtemp-consistent test-fake-getpid test-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
