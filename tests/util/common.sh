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

	echo RET1 $RET1
	echo RET2 $RET2
	echo CMP_OUT $CMP_OUT
	echo CMP_ERR $CMP_ERR
	echo CMP_COM $CMP_COM
	if [ $RET1 -ne $RET2 ] || [ $CMP_OUT -ne 0 ] || [ $CMP_ERR -ne 0 ] || [ $CMP_COM -ne 0 ]; then
	        exit 1
	fi

	return $RET1
}
