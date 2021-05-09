. "$(dirname -- "$0")"/../util/common.sh

is_acl_directory_traversal_override && exit $TEST_EX_SKIP

function cleanup() {
	chmod u+rwx "$ORIG_TMPDIR/permission-denied"
}

ORIG_TMPDIR="$TMPDIR"

mkdir -p "$TMPDIR/permission-denied/tmp"
trap cleanup EXIT
chmod 000 "$TMPDIR/permission-denied"

export TMPDIR="$TMPDIR/permission-denied/tmp"

run_test -q "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
