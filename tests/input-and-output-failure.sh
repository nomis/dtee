. "$(dirname "$0")"/util/common.sh

TEST_LD_PRELOAD="./libtest-socket-unix-failure.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
(umask 0333; touch file_not_writeable)
run_test "-o" "file_not_writeable" "$RUN"
RET=$?

variables_must_eq RET $EX_CANTCREAT
