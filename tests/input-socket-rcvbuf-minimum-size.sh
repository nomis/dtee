. "$(dirname "$0")"/util/common.sh

if [ "$UNAME" == "FreeBSD" ]; then
	export DTEE_TEST_GETSOCKOPT_RCVBUF="$((256 * 1024))"
else
	export DTEE_TEST_GETSOCKOPT_RCVBUF="PIPE_BUF"
fi

# If the outgoing SO_RCVBUF is exactly PIPE_BUF, it does not need to be modified
# The incoming SO_SNDBUF will be set to the same as the observed SO_RCVBUF
TEST_LD_PRELOAD="./libtest-socket-setsockopt-monitor.so:./libtest-socket-getsockopt-fake-rcvbuf.so:./libtest-mkdtemp-consistent.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
