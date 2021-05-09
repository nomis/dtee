. "$(dirname -- "$0")"/../util/common.sh

if [ ! -d "$TMPDIR/permission-denied" ]; then
	mkdir -p "$TMPDIR/permission-denied"
fi
chmod 000 "$TMPDIR/permission-denied"
export TMPDIR="$TMPDIR/permission-denied/tmp"

run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
