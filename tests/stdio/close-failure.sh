. "$(dirname -- "$0")"/../util/common.sh


export DTEE_TEST_DUP2_FAIL=parent
export DTEE_TEST_FILE_FAIL_NAME="/dev/null"
export DTEE_TEST_FILE_OPEN_FAIL=0
export DTEE_TEST_FILE_CLOSE_FAIL=1

TEST_LD_PRELOAD=(test-dup2-failure test-file-failures)
no_ld_preload "test-execvp-fd-check"

TEST_EXEC=./util/test-execvp-without-stdio
# Run with stderr missing, which should be reopened to /dev/null but dup2() and
# close() will fail. Run with stdin missing, which will require the use of
# dup2() to move the new open() file descriptor.
run_test IE ./dtee "$RUN"
RET=$?

variables_must_eq RET $EX_OSERR
