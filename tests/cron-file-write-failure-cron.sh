. "$(dirname "$0")"/util/common.sh

# If the temporary output file cannot be written, it should fallback to using stdout/stderr as normal
TEST_LD_PRELOAD="./libtest-mkostemp-readonly.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
run_test "-q" "$RUN"
RET=$?

# Output errors prevent a success exit status
variables_must_eq RET $EX_IOERR
