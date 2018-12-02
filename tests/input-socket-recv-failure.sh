. "$(dirname "$0")"/util/common.sh

# Allow the first few lines to work
case "$UNAME" in
	CYGWIN_*)
		# There are two extra recv calls during setup of the output sockets
		export DTEE_TEST_SOCKET_RECV_FAILURE_ALLOW=6
		;;

	*)
		export DTEE_TEST_SOCKET_RECV_FAILURE_ALLOW=4
		;;
esac

TEST_LD_PRELOAD=(test-socket-recv-failure)
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_IOERR
