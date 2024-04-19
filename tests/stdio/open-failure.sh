. "$(dirname -- "$0")"/../util/common.sh

export DTEE_TEST_FILE_FAIL_NAME="/dev/null"
export DTEE_TEST_FILE_OPEN_FAIL=1
export DTEE_TEST_FILE_CLOSE_FAIL=0

TEST_LD_PRELOAD=(test-file-failures)
no_ld_preload "test-execvp-fd-check"

TEST_EXEC=./util/test-execvp-without-stdio
# Run with stdout and stderr missing, which should be reopened to /dev/null but
# open() will fail.
run_test OE ./dtee "$RUN"
RET=$?

variables_must_eq RET $EX_OSERR
