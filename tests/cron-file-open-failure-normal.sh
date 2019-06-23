. "$(dirname "$0")"/util/common.sh

is_acl_override && exit $TEST_EX_SKIP

(umask 0333; touch file_not_writeable; chmod a-w file_not_writeable)
run_test "-q" "-o" "file_not_writeable" "$RUN"
RET=$?

variables_must_eq RET $EX_CANTCREAT
