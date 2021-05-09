. "$(dirname -- "$0")"/../util/common.sh

is_acl_directory_traversal_override && exit $TEST_EX_SKIP

mkdir -p "$TMPDIR/permission-denied/tmp"
chmod 000 "$TMPDIR/permission-denied"

export TMPDIR="$TMPDIR/permission-denied/tmp"

run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
