. "$(dirname -- "$0")"/../util/common.sh

# Ask for help, get exit code 0 and usage information (including cron option)
./dtee --help 1>"$TESTDIR/$NAME.out" 2>"$TESTDIR/$NAME.err"
RET=$?

USAGE=0
CRON=0
while read -r line; do
	echo LINE "$line"
	case "$line" in
	"Usage: ./dtee "*) USAGE=1 ;;
	*"operate in cron mode"*) CRON=1 ;;
	esac
done <"$TESTDIR/$NAME.out"

ERR=0
while read -r line; do
	ERR=1
done <"$TESTDIR/$NAME.err"

variables_must_eq RET $EXIT_SUCCESS \
	USAGE 1 \
	CRON 1 \
	ERR 0
