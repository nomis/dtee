. "$(dirname -- "$0")"/../util/common.sh

export DTEE_TEST_DUP2_FAIL=parent

TEST_LD_PRELOAD=(test-dup2-failure)
no_ld_preload "test-execvp-fd-check"

TEST_EXEC=./util/test-execvp-without-stdio
# Run with stderr missing, which should be reopened to /dev/null but dup2() will
# fail. Run with stdin missing, which will require the use of dup2() to move the
# new open() file descriptor.
run_test IE ./dtee "$RUN"
RET=$?

variables_must_eq RET $EX_OSERR
