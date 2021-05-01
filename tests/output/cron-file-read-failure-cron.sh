. "$(dirname "$0")"/../util/common.sh

# If the temporary output file cannot be read, it should output an error message
TEST_LD_PRELOAD=(test-mkostemp-writeonly test-fake-getpid test-fake-getuid)
run_test "-q" "$RUN"
RET=$?

# Output errors prevent a success exit status
variables_must_eq RET $EX_IOERR
