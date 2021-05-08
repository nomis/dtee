. "$(dirname -- "$0")"/../util/common.sh

TEST_LD_PRELOAD=(test-write-throw-exception)
# If there's an exception then dtee will flush buffered output and exit
run_test -q "$RUN"
RET=$?

variables_must_eq RET $EX_IOERR
