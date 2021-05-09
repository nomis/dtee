. "$(dirname -- "$0")"/../util/common.sh

is_acl_override && exit $TEST_EX_SKIP

rmdir "$TMPDIR/permission-denied"
(umask 0333; mkdir -p "$TMPDIR/permission-denied"; chmod a-w "$TMPDIR/permission-denied")
export TMPDIR="$TMPDIR/permission-denied/tmp"

run_test -q "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
