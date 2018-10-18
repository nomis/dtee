TESTDIR="dtee@test"
mkdir -p "$TESTDIR"

NAME="$(basename "$0")"
NAME="${NAME/.sh}"

# Ensure commands that are run have relative path names in argv[0]
rm -f "$TESTDIR/$NAME.run"
if [ -e "${0/.sh/.run}" ]; then
	ln -s "${0/.sh/.run}" "$TESTDIR/$NAME.run"
fi
RUN="$TESTDIR/$NAME.run"

COMMON_TEST_LD_PRELOAD=(./libexecvp-fd-check.so)

TEST_EXEC=./dtee
TEST_NO_STDIN=0

# Use a consistent and isolated temporary directory
rm -rf "$TESTDIR/$NAME.tmp"
mkdir -p "$TESTDIR/$NAME.tmp"
export TMPDIR="./$TESTDIR/$NAME.tmp"

# /usr/include/sysexits.h
EX_USAGE=64
EX_DATAERR=65
EX_NOINPUT=66
EX_NOUSER=67
EX_NOHOST=68
EX_UNAVAILABLE=69
EX_SOFTWARE=70
EX_OSERR=71
EX_OSFILE=72
EX_CANTCREAT=73
EX_IOERR=74
EX_TEMPFAIL=75
EX_PROTOCOL=76
EX_NOPERM=77
EX_CONFIG=78

function before_test() {
	OLD_LD_PRELOAD="$LD_PRELOAD"
	OIFS="$IFS" IFS=:
	NEW_LD_PRELOAD="${COMMON_TEST_LD_PRELOAD[*]}"
	IFS="$OIFS"
	if [ ! -z "$TEST_LD_PRELOAD" ]; then
		NEW_LD_PRELOAD="$TEST_LD_PRELOAD:$NEW_LD_PRELOAD"
	fi
	if [ -z "$OLD_LD_PRELOAD" ]; then
		export LD_PRELOAD="$NEW_LD_PRELOAD"
	else
		export LD_PRELOAD="$NEW_LD_PRELOAD:$OLD_LD_PRELOAD"
	fi
}

function after_test() {
	export LD_PRELOAD="$OLD_LD_PRELOAD"
}

function cmp_files() {
	cmp "$1" "$2"
	CMP=$?
	[ $CMP -ne 0 ] && diff -U4 "$1" "$2"
	return $CMP
}

function check_variables() {
	CV_EXIT_CODE="$1"
	CV_OPERATOR="$2"
	CV_OK=1
	shift 2

	while [ -n "$1" ]; do
		echo "$1" actual "${!1}" expected "$2"
		if [ ! "${!1}" $CV_OPERATOR "$2" ]; then
			CV_OK=0
		fi
		shift 2
	done

	if [ $CV_OK -eq 1 ]; then
		exit "$CV_EXIT_CODE"
	fi
}

function check_variables_eq() {
	check_variables 0 -eq "$@"
}

function variables_must_eq() {
	check_variables_eq "$@"
	exit 1
}

function check_variables_ne() {
	check_variables 1 -ne "$@"
}

function run_test() {
	if [ -e "${0/.sh/.in.txt}" ]; then
		STDIN_FILE="${0/.sh/.in.txt}"
	else
		STDIN_FILE="/dev/null"
	fi

	declare -f test_prepare >/dev/null && test_prepare

	rm -f "$TESTDIR/$NAME.out.txt" "$TESTDIR/$NAME.err.txt"
	before_test
	if [ $TEST_NO_STDIN -eq 0 ]; then
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt"
	else
		"$TEST_EXEC" "$@" <&- 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt"
	fi
	RET1=$?
	after_test

	cmp_files "${0/.sh/.out.txt}" "$TESTDIR/$NAME.out.txt"
	CMP_OUT=$?

	cmp_files "${0/.sh/.err.txt}" "$TESTDIR/$NAME.err.txt"
	CMP_ERR=$?

	declare -f test_cleanup >/dev/null && test_cleanup
	declare -f test_prepare >/dev/null && test_prepare

	rm -f "$TESTDIR/$NAME.com.txt"
	before_test
	if [ $TEST_NO_STDIN -eq 0 ]; then
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.com.txt" 2>&1
	else
		"$TEST_EXEC" "$@" <&- 1>"$TESTDIR/$NAME.com.txt" 2>&1
	fi
	RET2=$?
	after_test

	cmp_files "${0/.sh/.com.txt}" "$TESTDIR/$NAME.com.txt"
	CMP_COM=$?

	declare -f test_cleanup >/dev/null && test_cleanup

	echo RET2 $RET2
	check_variables_ne RET1 $RET2 CMP_OUT 0 CMP_ERR 0 CMP_COM 0

	return $RET1
}
