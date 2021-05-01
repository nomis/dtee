. "$(dirname "$0")"/../util/common.sh

is_acl_override && exit $TEST_EX_SKIP

TEST_LD_PRELOAD=(test-socket-unix-failure test-fake-getpid test-fake-getuid)
rm -f input_and_output_failure
(umask 0333; touch input_and_output_failure; chmod a-w input_and_output_failure)
run_test "-o" "input_and_output_failure" "$RUN"
RET=$?

variables_must_eq RET $EX_CANTCREAT
