. "$(dirname -- "$0")"/../util/common.sh

no_ld_preload "test-execvp-fd-check"
TEST_EXEC=./util/test-execvp-without-stdio
# Run with stdin missing, which should have no impact
run_test I ./dtee "$RUN"
RET=$?

variables_must_eq RET 42
