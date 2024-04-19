. "$(dirname -- "$0")"/../util/common.sh

no_ld_preload "test-execvp-fd-check"
TEST_EXEC=./util/test-execvp-without-stdio
# Run with stdin missing, which should be ignored
# Run with stderr missing, which should be reopened to /dev/null
run_test IE ./dtee "$RUN"
RET=$?

variables_must_eq RET 42
