. "$(dirname "$0")"/util/common.sh
set -x

TEST_LD_PRELOAD=(test-socket-connect-failure test-fake-getpid test-fake-getuid)
run_test "$RUN"
RET=$?

set +x
variables_must_eq RET $EX_UNAVAILABLE
