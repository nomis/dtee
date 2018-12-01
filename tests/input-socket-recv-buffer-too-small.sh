. "$(dirname "$0")"/util/common.sh

if [ "$UNAME" == "GNU" ]; then
	exit $TEST_EX_SKIP
fi

# Datagram sockets truncate the trailing portion if the buffer is too small
TEST_LD_PRELOAD=(test-socket-recv-buffer-too-small)
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
