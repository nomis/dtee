. "$(dirname "$0")"/util/common.sh

is_acl_override && exit $TEST_EX_SKIP

(umask 0333; touch file_not_writeable)
run_test "-o" "file_not_writeable" "$RUN"
RET=$?

variables_must_eq RET $EX_CANTCREAT
