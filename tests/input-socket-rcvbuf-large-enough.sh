. "$(dirname "$0")"/util/common.sh

if [ "$UNAME" == "FreeBSD" ] || [ "$UNAME" == "OpenBSD" ] || [ "$UNAME" == "DragonFly" ]; then
	export DTEE_TEST_GETSOCKOPT_RCVBUF="$((1024 * 1024))"
elif [ "$UNAME" == "NetBSD" ]; then
	export DTEE_TEST_GETSOCKOPT_RCVBUF="$((192 * 1024))"
elif [ "$UNAME" == "GNU" ]; then
	exit $TEST_EX_SKIP
else
	export DTEE_TEST_GETSOCKOPT_RCVBUF="PIPE_BUF*4"
fi

# If the outgoing SO_RCVBUF is larger than PIPE_BUF, it does not need to be modified
# The incoming SO_SNDBUF will be set to the same as the observed SO_RCVBUF
TEST_LD_PRELOAD="./libtest-socket-setsockopt-monitor.so:./libtest-socket-getsockopt-fake-rcvbuf.so:./libtest-mkdtemp-consistent.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
