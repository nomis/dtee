. "$(dirname -- "$0")"/../util/common.sh

export TMPDIR="$TMPDIR/not-found"

run_test -q "$RUN"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
