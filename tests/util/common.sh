BASEDIR="$(dirname "$0")"
UTILDIR="$BASEDIR/util"
NAME="$(basename "$0")"
NAME="${NAME/.sh}"

if [ ! -z "$1" ]; then
	NAME="${NAME},$1"
fi

GROUP=""
dir="$BASEDIR"
while [ "$(basename "$dir")" != "tests" ]; do
	[ ! -z "$GROUP" ] && GROUP="/$GROUP"
	GROUP="$(basename "$dir")$GROUP"
	dir="$(dirname "$dir")"
done

cd tests || exit 1
ln -sf ../dtee dtee
ln -sf ../cronty cronty
BASETESTDIR="dtee@test"
TESTDIR="$BASETESTDIR"
if [ "$GROUP" != "" ]; then
	TESTDIR="$BASETESTDIR/$GROUP"
fi
mkdir -p "$TESTDIR"

# GNU standard
TEST_EX_OK=0
TEST_EX_FAIL=1
TEST_EX_SKIP=77

# C standard
EXIT_SUCCESS=0
EXIT_FAILURE=1

# Ensure commands that are run have relative path names in argv[0]
rm -f "$TESTDIR/$NAME.run" || exit $TEST_EX_FAIL
if [ -e "${0/.sh/.run}" ]; then
	ln "${0/.sh/.run}" "$TESTDIR/$NAME.run" || exit $TEST_EX_FAIL
fi
RUN="$TESTDIR/$NAME.run"

COMMON_TEST_LD_PRELOAD=(test-execvp-fd-check test-fork-sigchld-check test-fake-strerror test-fake-strsignal)

TEST_EXEC=./dtee
TEST_NO_STDIN=0
TEST_EXTRA_OUTPUT=0
TEST_ALT_STDOUT=
TEST_ALT_STDERR=

# Use a consistent and isolated temporary directory
rm -rf "$TESTDIR/$NAME.tmp" || exit $TEST_EX_FAIL
mkdir -p "$TESTDIR/$NAME.tmp" || exit $TEST_EX_FAIL
export TMPDIR="./$TESTDIR/$NAME.tmp"

# /usr/include/sysexits.h
. ../sysexits.txt

# Signals
. ../signal.txt

# Version
. ./util/version.txt

UNAME="$(uname)"
SHORT_UNAME="$UNAME"

case "$UNAME" in
	Darwin)
		SHLIB_PREFIX="./util/lib"
		SHLIB_EXT=.dylib
		;;
	CYGWIN_*)
		SHLIB_PREFIX="./util/cyg"
		SHLIB_EXT=.dll
		SHORT_UNAME="CYGWIN"

		# Windows can't find DLLs unless they're in the executable's
		# directory or the PATH. This causes preloaded libraries to
		# fail to find their dependencies. Make a link to all of them
		# in the current directory.
		ln -st . ./util/cyg*.dll 2>/dev/null
		;;
	*)
		SHLIB_PREFIX="./util/lib"
		SHLIB_EXT=.so
		;;
esac

function is_acl_override() {
	# Are ACL permissions on files overridden?
	case "$UNAME" in
		CYGWIN_*)
			# Running with elevated permissions
			id -G | grep -E '\<(544)\>'
			return $?
			;;

		*)
			# Running as root
			return [ $(id -u) -eq 0 ]
			;;
	esac
}

function no_ld_preload() {
	NEW_ARRAY=()
	count=${#COMMON_TEST_LD_PRELOAD[@]}
	for ((i = 0; i < count; i++)); do
		if [ "${COMMON_TEST_LD_PRELOAD[i]}" != "$1" ] ; then
			NEW_ARRAY+=(${COMMON_TEST_LD_PRELOAD[i]})
		fi
	done
	COMMON_TEST_LD_PRELOAD=$NEW_ARRAY
}

function get_ld_preload() {
	case "$UNAME" in
		Darwin)
			echo "$DYLD_INSERT_LIBRARIES"
			;;

		*)
			echo "$LD_PRELOAD"
			;;
	esac
}

function set_ld_preload() {
	case "$UNAME" in
		Darwin)
			if [ -n "$1" ]; then
				export DYLD_INSERT_LIBRARIES="$1"
			else
				unset DYLD_INSERT_LIBRARIES
			fi
			;;

		*)
			if [ -n "$1" ]; then
				export LD_PRELOAD="$1"
			else
				unset LD_PRELOAD
			fi
			;;
	esac
}

function build_ld_preload() {
	LD_PRELOAD_STR="$(get_ld_preload)"
	count=${#TEST_LD_PRELOAD[@]}
	for ((i = 0; i < count; i++)); do
		if [ -n "$LD_PRELOAD_STR" ]; then
			LD_PRELOAD_STR="$LD_PRELOAD_STR:"
		fi
		LD_PRELOAD_STR="${LD_PRELOAD_STR}${SHLIB_PREFIX}${TEST_LD_PRELOAD[i]}${SHLIB_EXT}"
	done
	count=${#COMMON_TEST_LD_PRELOAD[@]}
	for ((i = 0; i < count; i++)); do
		if [ -n "$LD_PRELOAD_STR" ]; then
			LD_PRELOAD_STR="$LD_PRELOAD_STR:"
		fi
		LD_PRELOAD_STR="${LD_PRELOAD_STR}${SHLIB_PREFIX}${COMMON_TEST_LD_PRELOAD[i]}${SHLIB_EXT}"
	done
	echo "$LD_PRELOAD_STR"
}

function periodic_cleanup() {
	if [ "$UNAME" == "Darwin" ] && [ "$DTEE_TEST_COREDUMPS" == "1" ]; then
		# Why dump the whole of RAM without using sparse files? 😖
		rm -f /cores/core.* /private/cores/core.*
	fi
}

function before_test() {
	OLD_LD_PRELOAD="$(get_ld_preload)"
	NEW_LD_PRELOAD="$(build_ld_preload)"
	set_ld_preload "$NEW_LD_PRELOAD"
}

function after_test() {
	set_ld_preload "$OLD_LD_PRELOAD"
}

function cmp_files() {
	EXPECTED_TEXT="$BASEDIR/$NAME.$1.txt"
	EXPECTED_EVAL="$BASEDIR/$NAME.$1.eval.txt"
	EXPECTED_TEXT_UNAME="$BASEDIR/$NAME.$1.$SHORT_UNAME.txt"
	EXPECTED_EVAL_UNAME="$BASEDIR/$NAME.$1.eval.$SHORT_UNAME.txt"
	ACTUAL="$TESTDIR/$NAME.$1.txt"

	if [ -e "$EXPECTED_TEXT_UNAME" ]; then
		EXPECTED_TEXT="$EXPECTED_TEXT_UNAME"
	fi
	if [ -e "$EXPECTED_EVAL_UNAME" ]; then
		EXPECTED_EVAL="$EXPECTED_EVAL_UNAME"
	fi

	if [ -e "$EXPECTED_TEXT" ]; then
		EXPECTED="$EXPECTED_TEXT"
	elif [ -e "$EXPECTED_EVAL" ]; then
		EXPECTED="$TESTDIR/$NAME.$1.expected.txt"

		rm -f "$EXPECTED"
		OLD_IFS="$IFS"
		IFS=""
		while read -r line; do
			eval echo "$line"
		done < "$EXPECTED_EVAL" > "$EXPECTED"
		IFS="$OLD_IFS"
	else
		echo "Missing file $EXPECTED_TEXT or $EXPECTED_EVAL or $EXPECTED_TEXT_UNAME or $EXPECTED_EVAL_UNAME"
		diff -U4 /dev/null "$ACTUAL"
		return 1
	fi

	cmp "$EXPECTED" "$ACTUAL"
	CMP=$?
	[ $CMP -ne 0 ] && diff -U4 "$EXPECTED" "$ACTUAL"
	return $CMP
}

function make_fifo() {
	rm -f "$TESTDIR/$NAME.$1.fifo" || exit $TEST_EX_FAIL
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
		if [ -z "${!CV_ACTUAL}" ]; then
			CV_RET=1
		elif [ -z "$CV_EXPECTED" ]; then
			CV_RET=1
		elif [ "${!CV_ACTUAL}" != "$CV_EXPECTED" ]; then
			CV_RET=1
		fi
		shift 2
		if [ $? -ne 0 ]; then
			echo "Invalid list of variables"
			CV_RET=1
			break
		fi
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
	if [ -e "$BASEDIR/$NAME.in.txt" ]; then
		STDIN_FILE="$BASEDIR/$NAME.in.txt"
	else
		STDIN_FILE="/dev/null"
	fi

	declare -f test_prepare >/dev/null && test_prepare

	rm -f "$TESTDIR/$NAME.out.txt" "$TESTDIR/$NAME.err.txt"
	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		rm -f "$TESTDIR/$NAME.extra-out.txt"
	fi
	periodic_cleanup
	if [ $TEST_NO_STDIN -eq 1 ]; then
		before_test
		"$TEST_EXEC" "$@" <&- 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt"
		RET1=$?
		after_test
	elif [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		FIFO=$(make_fifo "extra-out1")
		before_test
		DTEE_TEST_EXTRA_OUTPUT_FD=3 "$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt" 3>"$FIFO" &
		PID=$!
		after_test

		cat <"$FIFO" >"$TESTDIR/$NAME.extra-out.txt"

		wait $PID
		RET1=$?
	elif [ -n "$TEST_ALT_STDOUT" ]; then
		before_test
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TEST_ALT_STDOUT" 2>"$TESTDIR/$NAME.err.txt"
		RET1=$?
		after_test
	elif [ -n "$TEST_ALT_STDERR" ]; then
		before_test
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.out.txt" 2>"$TEST_ALT_STDERR"
		RET1=$?
		after_test
	else
		before_test
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt"
		RET1=$?
		after_test
	fi
	periodic_cleanup

	if [ -z "$TEST_ALT_STDOUT" ]; then
		cmp_files "out"
		CMP_OUT=$?
	fi

	if [ -z "$TEST_ALT_STDERR" ]; then
		cmp_files "err"
		CMP_ERR=$?
	fi

	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		cmp_files "extra-out"
		CMP_EXTRA_OUT1=$?
	fi

	# Only run these once because combined output is meaningless
	if [ -n "$TEST_ALT_STDOUT" ]; then
		check_variables_eq CMP_ERR 0
		return $RET1
	elif [ -n "$TEST_ALT_STDERR" ]; then
		check_variables_eq CMP_OUT 0
		return $RET1
	fi

	declare -f test_cleanup >/dev/null && test_cleanup
	declare -f test_prepare >/dev/null && test_prepare

	rm -f "$TESTDIR/$NAME.com.txt" "$TESTDIR/$NAME.extra-out.txt"
	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		rm -f "$TESTDIR/$NAME.extra-out.txt"
	fi
	periodic_cleanup
	if [ $TEST_NO_STDIN -eq 1 ]; then
		before_test
		"$TEST_EXEC" "$@" <&- 1>"$TESTDIR/$NAME.com.txt" 2>&1
		RET2=$?
		after_test
	elif [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		FIFO=$(make_fifo "extra-out2")
		before_test
		DTEE_TEST_EXTRA_OUTPUT_FD=3 "$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.com.txt" 2>&1 3>"$FIFO" &
		PID=$!
		after_test

		cat <"$FIFO" >"$TESTDIR/$NAME.extra-out.txt"

		wait $PID
		RET2=$?
	else
		before_test
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.com.txt" 2>&1
		RET2=$?
		after_test
	fi
	periodic_cleanup

	cmp_files "com"
	CMP_COM=$?

	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		cmp_files "extra-out"
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

function run_with_preload() {
	before_test
	"$@"
	after_test
}

set -x
