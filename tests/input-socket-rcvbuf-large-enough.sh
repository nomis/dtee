. "$(dirname "$0")"/util/common.sh

if [ "$SHORT_UNAME" == "CYGWIN" ]; then
	export DTEE_TEST_GETSOCKOPT_RCVBUF="$((4 * 1024 * 1024))"
elif [ "$UNAME" == "FreeBSD" ] || [ "$UNAME" == "OpenBSD" ] || [ "$UNAME" == "DragonFly" ] || [ "$UNAME" == "Darwin" ]; then
	export DTEE_TEST_GETSOCKOPT_RCVBUF="$((1024 * 1024))"
elif [ "$UNAME" == "NetBSD" ]; then
	export DTEE_TEST_GETSOCKOPT_RCVBUF="$((192 * 1024))"
elif [ "$UNAME" == "GNU" ]; then
	exit $TEST_EX_SKIP
else
	export DTEE_TEST_GETSOCKOPT_RCVBUF="max(PIPE_BUF,BUFSIZ)*4"
fi

# If the outgoing SO_RCVBUF is larger than max(PIPE_BUF,BUFSIZ), it does not need to be modified
# The incoming SO_SNDBUF will be set to the same as the observed SO_RCVBUF
TEST_LD_PRELOAD=(test-socket-setsockopt-monitor test-socket-getsockopt-fake-rcvbuf test-mkdtemp-consistent test-fake-getpid test-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
