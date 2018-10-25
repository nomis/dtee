. "$(dirname "$0")"/util/common.sh

run_test "-q" "-o" "/dev/full" "$RUN"
RET=$?

variables_must_eq RET $EX_IOERR
