. "$(dirname "$0")"/../util/common.sh
TEST_LANGUAGE="$1"
. "$(dirname "$0")"/../util/locale.sh

TEST_LD_PRELOAD=(test-bindtextdomain-override)

# Run a process in cron mode that exits with an error (translated)
LANGUAGE="$TEST_LANGUAGE" LANG= LC_ALL="$FOUND_LOCALE" run_test "-q" "$RUN"
RET=$?

variables_must_eq RET 42
