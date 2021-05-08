. "$(dirname -- "$0")"/../util/common.sh

TEST_LD_PRELOAD=(test-write-throw-exception)
# If there's an exception then dtee will exit
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_IOERR
