. "$(dirname "$0")"/util/common.sh
TEST_LANGUAGE="$1"
. "$(dirname "$0")"/util/locale.sh

TEST_LD_PRELOAD=(test-bindtextdomain-override)

# Ask for help, get exit code 0 and usage information (including cron option) (translated)
LANGUAGE="$TEST_LANGUAGE" LANG= LC_ALL="$FOUND_LOCALE" run_test --help
RET=$?

variables_must_eq RET $EXIT_SUCCESS
