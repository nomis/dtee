. "$(dirname -- "$0")"/../util/common.sh

TEST_LD_PRELOAD=(test-write-throw-exception)
# If there's an exception then dtee will flush buffered output and exit
run_test -q ./util/test-lorem-ipsum-late-exit 1 THROW
RET=$?

variables_must_eq RET $EX_IOERR
