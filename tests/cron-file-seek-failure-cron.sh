. "$(dirname "$0")"/util/common.sh

# If the temporary output file cannot be read, it should output an error message
TEST_LD_PRELOAD=(./libtest-mkostemp-consistent ./libtest-lseek-failure ./libtest-fake-getpid ./libtest-fake-getuid)
run_test "-q" "$RUN"
RET=$?

# Output errors prevent a success exit status
variables_must_eq RET $EX_IOERR
