. "$(dirname -- "$0")"/../util/common.sh

TEST_EXEC=./util/test-execvp-without-stdio
# Run with stdout missing, which should be reopened to /dev/null
run_test O ./dtee "$RUN"
RET=$?

variables_must_eq RET 42
