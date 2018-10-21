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

COMMON_TEST_LD_PRELOAD=(./libtest-execvp-fd-check.so)

TEST_EXEC=./dtee
TEST_NO_STDIN=0
TEST_EXTRA_OUTPUT=0

# Use a consistent and isolated temporary directory
rm -rf "$TESTDIR/$NAME.tmp"
mkdir -p "$TESTDIR/$NAME.tmp"
export TMPDIR="./$TESTDIR/$NAME.tmp"

# GNU standard
TEST_EX_OK=0
TEST_EX_FAIL=1
TEST_EX_SKIP=77

# C standard
EXIT_SUCCESS=0
EXIT_FAILURE=1

# /usr/include/sysexits.h
EX_OK=0
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

function make_fifo() {
	rm -f "$TESTDIR/$NAME.$1.fifo"
	mkfifo "$TESTDIR/$NAME.$1.fifo" || exit $TEST_EX_FAIL
	echo "$TESTDIR/$NAME.$1.fifo"
}

function check_variables() {
	set +x
	CV_RET=0

	while [ -n "$1" ]; do
		CV_ACTUAL="$1"
		CV_EXPECTED="$2"
		echo "$CV_ACTUAL" actual "${!CV_ACTUAL}" expected "$CV_EXPECTED"
		if [ ${!CV_ACTUAL} -ne $CV_EXPECTED ]; then
			CV_RET=1
		fi
		shift 2
	done

	set -x
	return $CV_RET
}

function variables_must_eq() {
	set +x
	check_variables "$@" || exit $TEST_EX_FAIL
	exit $TEXT_EX_OK
}

function check_variables_eq() {
	set +x
	check_variables "$@" || exit $TEST_EX_FAIL
}

function run_test() {
	if [ -e "${0/.sh/.in.txt}" ]; then
		STDIN_FILE="${0/.sh/.in.txt}"
	else
		STDIN_FILE="/dev/null"
	fi

	declare -f test_prepare >/dev/null && test_prepare

	rm -f "$TESTDIR/$NAME.out.txt" "$TESTDIR/$NAME.err.txt"
	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		rm -f "$TESTDIR/$NAME.extra-out.txt"
	fi
	before_test
	if [ $TEST_NO_STDIN -eq 1  ]; then
		"$TEST_EXEC" "$@" <&- 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt"
		RET1=$?
	elif [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		FIFO=$(make_fifo "extra-out")
		DTEE_TEST_EXTRA_OUTPUT_FD=3 "$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt" 3>"$FIFO" &
		PID=$!

		cat <"$FIFO" >"$TESTDIR/$NAME.extra-out.txt"

		wait $PID
		RET1=$?
	else
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt"
		RET1=$?
	fi
	after_test

	cmp_files "${0/.sh/.out.txt}" "$TESTDIR/$NAME.out.txt"
	CMP_OUT=$?

	cmp_files "${0/.sh/.err.txt}" "$TESTDIR/$NAME.err.txt"
	CMP_ERR=$?

	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		cmp_files "${0/.sh/.extra-out.txt}" "$TESTDIR/$NAME.extra-out.txt"
		CMP_EXTRA_OUT1=$?
	fi

	declare -f test_cleanup >/dev/null && test_cleanup
	declare -f test_prepare >/dev/null && test_prepare

	rm -f "$TESTDIR/$NAME.com.txt" "$TESTDIR/$NAME.extra-out.txt"
	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		rm -f "$TESTDIR/$NAME.extra-out.txt"
		mkfifo "$TESTDIR/$NAME.extra-out.fifo"
	fi
	before_test
	if [ $TEST_NO_STDIN -eq 1  ]; then
		"$TEST_EXEC" "$@" <&- 1>"$TESTDIR/$NAME.com.txt" 2>&1
		RET2=$?
	elif [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		FIFO=$(make_fifo "extra-out")
		DTEE_TEST_EXTRA_OUTPUT_FD=3 "$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.com.txt" 2>&1 3>"$FIFO" &
		PID=$!

		cat <"$FIFO" >"$TESTDIR/$NAME.extra-out.txt"

		wait $PID
		RET2=$?
	else
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.com.txt" 2>&1
		RET2=$?
	fi
	after_test

	cmp_files "${0/.sh/.com.txt}" "$TESTDIR/$NAME.com.txt"
	CMP_COM=$?

	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		cmp_files "${0/.sh/.extra-out.txt}" "$TESTDIR/$NAME.extra-out.txt"
		CMP_EXTRA_OUT2=$?
	fi

	declare -f test_cleanup >/dev/null && test_cleanup

	echo RET2 $RET2
	check_variables_eq RET1 $RET2 CMP_OUT 0 CMP_ERR 0 CMP_COM 0
	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		check_variables_eq CMP_EXTRA_OUT1 0 CMP_EXTRA_OUT2 0
	fi

	return $RET1
}

set -x
