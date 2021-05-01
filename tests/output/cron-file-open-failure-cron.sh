. "$(dirname "$0")"/../util/common.sh

# If the temporary output file cannot be opened, it should fallback to using stdout/stderr as normal
TEST_LD_PRELOAD=(test-mkostemp-failure test-fake-getpid test-fake-getuid)
run_test "-q" "$RUN"
RET=$?

# Output errors prevent a success exit status
variables_must_eq RET $EX_CANTCREAT
