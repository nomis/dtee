. "$(dirname "$0")"/util/common.sh

is_acl_override && exit $TEST_EX_SKIP

rm -f cron_file_open_failure_normal
(umask 0333; touch cron_file_open_failure_normal; chmod a-w cron_file_open_failure_normal)
run_test "-q" "-o" "cron_file_open_failure_normal" "$RUN"
RET=$?

variables_must_eq RET $EX_CANTCREAT
