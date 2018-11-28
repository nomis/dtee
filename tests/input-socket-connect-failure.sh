. "$(dirname "$0")"/util/common.sh
set -x

TEST_LD_PRELOAD=(./libtest-socket-connect-failure ./libtest-fake-getpid ./libtest-fake-getuid)
run_test "$RUN"
RET=$?

set +x
variables_must_eq RET $EX_UNAVAILABLE
