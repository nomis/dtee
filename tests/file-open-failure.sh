. "$(dirname "$0")"/util/common.sh

is_acl_override && exit $TEST_EX_SKIP

rm -f file_open_failure
(umask 0333; touch file_open_failure; chmod a-w file_open_failure)
run_test "-o" "file_open_failure" "$RUN"
RET=$?

variables_must_eq RET $EX_CANTCREAT
