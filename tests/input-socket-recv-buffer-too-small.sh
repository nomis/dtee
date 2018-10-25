. "$(dirname "$0")"/util/common.sh

# Datagram sockets truncate the trailing portion if the buffer is too small
TEST_LD_PRELOAD="./libtest-socket-recv-buffer-too-small.so"
run_test "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
