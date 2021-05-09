. "$(dirname -- "$0")"/../util/common.sh

export TMPDIR="$TMPDIR/not-found"

run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
