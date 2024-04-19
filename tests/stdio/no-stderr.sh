. "$(dirname -- "$0")"/../util/common.sh

TEST_EXEC=./util/test-execvp-without-stdio
# Run with stderr missing, which should be reopened to /dev/null
run_test E ./dtee "$RUN"
RET=$?

variables_must_eq RET 42
