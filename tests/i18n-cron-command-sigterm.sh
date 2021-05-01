. "$(dirname "$0")"/util/common.sh
TEST_LANGUAGE="$1"
. "$(dirname "$0")"/util/locale.sh

TEST_LD_PRELOAD=(test-bindtextdomain-override)

# Run a process in cron mode that is killed by a signal (translated)
LANGUAGE="$TEST_LANGUAGE" LANG= LC_ALL="$FOUND_LOCALE" run_test "-q" ./util/test-kill-pid $SIGTERM quiet
RET=$?

eval $(LANGUAGE="$TEST_LANGUAGE" LANG= LC_ALL="$FOUND_LOCALE" ./util/test-waitpid ./dtee ./dtee "-q" ./util/test-kill-pid $SIGTERM quiet)
RET2=$?

variables_must_eq RET $((128 + $SIGTERM)) \
	RET2 0 \
	WIFSIGNALED 0
