. "$(dirname "$0")"/util/common.sh

if [ "$UNAME" == "FreeBSD" ] || [ "$UNAME" == "OpenBSD" ] || [ "$UNAME" == "DragonFly" ]; then
	export DTEE_TEST_GETSOCKOPT_RCVBUF="$((512 * 1024))"
elif [ "$UNAME" == "NetBSD" ]; then
	export DTEE_TEST_GETSOCKOPT_RCVBUF="$((128 * 1024))"
elif [ "$UNAME" == "GNU" ]; then
	exit $TEST_EX_SKIP
else
	export DTEE_TEST_GETSOCKOPT_RCVBUF="max(PIPE_BUF,BUFSIZ)"
fi

# If the outgoing SO_RCVBUF is exactly max(PIPE_BUF,BUFSIZ), it does not need to be modified
# The incoming SO_SNDBUF will be set to the same as the observed SO_RCVBUF
TEST_LD_PRELOAD=(./libtest-socket-setsockopt-monitor ./libtest-socket-getsockopt-fake-rcvbuf ./libtest-mkdtemp-consistent ./libtest-fake-getpid ./libtest-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
