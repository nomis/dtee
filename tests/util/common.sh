BASEDIR="$(dirname -- "$0")"
UTILDIR="$BASEDIR/../util"
. "$UTILDIR/preload.sh"

NAME="$(basename -- "$0")"
NAME="${NAME%*.sh}"

if [ ! -z "$1" ]; then
	NAME="${NAME},$1"
fi

GROUP=""
dir="$BASEDIR"
while [ "$(basename -- "$dir")" != "tests" ]; do
	[ ! -z "$GROUP" ] && GROUP="/$GROUP"
	GROUP="$(basename -- "$dir")$GROUP"
	dir="$(dirname -- "$dir")"
done

ln -s ../dtee tests/dtee
ln -s dtee tests/cronty
BASETESTDIR="dtee@test"
TESTDIR="$BASETESTDIR"
if [ "$GROUP" != "" ]; then
	TESTDIR="$BASETESTDIR/$GROUP"
fi
mkdir -p "tests/$TESTDIR"

# GNU standard
TEST_EX_OK=0
TEST_EX_FAIL=1
TEST_EX_SKIP=77

# C standard
EXIT_SUCCESS=0
EXIT_FAILURE=1

function __portable_realpath() {
	TARGET="$1"
	pushd -- "$(dirname -- "$TARGET")" >/dev/null || exit $TEST_EX_FAIL
	TARGET="$(pwd)/$(basename -- "$TARGET")"
	while [ -L "$TARGET" ]; do
		TARGET="$(readlink -- "$TARGET")"
		[ $? -eq 0 ] || exit $TEST_EX_FAIL
		pushd -- "$(dirname -- "$TARGET")" >/dev/null || exit $TEST_EX_FAIL
		TARGET="$(pwd)/$(basename -- "$TARGET")"
		popd >/dev/null || exit $TEST_EX_FAIL
	done
	popd >/dev/null || exit $TEST_EX_FAIL
	echo "$TARGET"
}

# Ensure commands that are run have relative path names in argv[0]
rm -f "tests/$TESTDIR/$NAME.run" || exit $TEST_EX_FAIL
if [ -e "${0%*.sh}.run" ]; then
	RUN="$(__portable_realpath "${0%*.sh}.run")"
	[ $? -eq 0 ] || exit $TEST_EX_FAIL
	ln -- "$RUN" "tests/$TESTDIR/$NAME.run" || exit $TEST_EX_FAIL
fi
RUN="$TESTDIR/$NAME.run"

cd tests  || exit 1

TEST_EXEC=./dtee
TEST_NO_STDIN=0
TEST_EXTRA_OUTPUT=0
TEST_ALT_STDOUT=
TEST_ALT_STDERR=

# Use a consistent and isolated temporary directory
if [ -e "$TESTDIR/$NAME.tmp" ]; then
	chmod -R u+rwx "$TESTDIR/$NAME.tmp" || exit $TEST_EX_FAIL
	rm -rf "$TESTDIR/$NAME.tmp" || exit $TEST_EX_FAIL
fi
mkdir -p "$TESTDIR/$NAME.tmp" || exit $TEST_EX_FAIL
export TMPDIR="./$TESTDIR/$NAME.tmp"

# boost::system::error_code.what()
. ./util/boost-error_code-what.txt

# /usr/include/sysexits.h
. ./util/sysexits.txt

# Signals
. ./util/signal.txt

# Version
. ./util/version.txt

UNAME="$(uname)"
SHORT_UNAME="$UNAME"

case "$UNAME" in
	CYGWIN_*)
		SHORT_UNAME="CYGWIN"
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

		Linux)
			# Running with CAP_DAC_OVERRIDE
			grep -E '^CapEff:\s+.+[2367abef]$' /proc/self/status >/dev/null
			return $?
			;;

		*)
			# Running as root
			if [ "$(id -u)" = "0" ]; then
				return 0
			else
				return 1
			fi
			;;
	esac
}

function is_acl_directory_traversal_override() {
	# Is -x on directories ineffective?
	case "$UNAME" in
		CYGWIN_*)
			return 0
			;;

		Linux)
			# Running with CAP_DAC_OVERRIDE or CAP_DAC_READ_SEARCH
			grep -E '^CapEff:\s+.+[234567abcdef]$' /proc/self/status >/dev/null
			return $?
			;;

		*)
			is_acl_override
			return $?
			;;
	esac
}

function __periodic_cleanup() {
	if [ "$UNAME" == "Darwin" ] && [ "$DTEE_TEST_COREDUMPS" == "1" ]; then
		# Why dump the whole of RAM without using sparse files? 😖
		rm -f /cores/core.* /private/cores/core.*
	fi
}



function cmp_files() {
	set +x
	EXPECTED_TEXT="$BASEDIR/$NAME.$1.txt"
	EXPECTED_EVAL="$BASEDIR/$NAME.$1.eval.txt"
	EXPECTED_REGEX="$BASEDIR/$NAME.$1.regex.txt"
	EXPECTED_TEXT_UNAME="$BASEDIR/$NAME.$1.$SHORT_UNAME.txt"
	EXPECTED_EVAL_UNAME="$BASEDIR/$NAME.$1.eval.$SHORT_UNAME.txt"
	EXPECTED_REGEX_UNAME="$BASEDIR/$NAME.$1.regex.$SHORT_UNAME.txt"
	ACTUAL="$TESTDIR/$NAME.$1.txt"

	if [ -e "$EXPECTED_TEXT_UNAME" ]; then
		EXPECTED_TEXT="$EXPECTED_TEXT_UNAME"
	fi
	if [ -e "$EXPECTED_EVAL_UNAME" ]; then
		EXPECTED_EVAL="$EXPECTED_EVAL_UNAME"
	fi
	if [ -e "$EXPECTED_REGEX_UNAME" ]; then
		EXPECTED_REGEX="$EXPECTED_REGEX_UNAME"
	fi

	if [ -e "$EXPECTED_TEXT" ]; then
		EXPECTED="$EXPECTED_TEXT"
	elif [ -e "$EXPECTED_EVAL" ]; then
		EXPECTED="$TESTDIR/$NAME.$1.expected.txt"

		rm -f "$EXPECTED"
		OLD_IFS="$IFS"
		IFS=""
		while read -r line; do
			eval echo "$line" || return 1
		done < "$EXPECTED_EVAL" > "$EXPECTED"
		IFS="$OLD_IFS"
	elif [ -e "$EXPECTED_REGEX" ]; then
		EXPECTED="$TESTDIR/$NAME.$1.expected.txt"

		rm -f "$EXPECTED"
		OLD_IFS="$IFS"
		IFS=""
		exec {actual_fd}<"$ACTUAL"
		while read -r regex_line; do
			actual_line=
			read -r actual_line <&${actual_fd}
			if [ $? -eq 0 ]; then
				if [[ "$actual_line" =~ ^${regex_line}$ ]]; then
					echo "$actual_line"
				else
					echo "$regex_line"
				fi
			else
				echo "$regex_line"
			fi
		done < "$EXPECTED_REGEX" > "$EXPECTED"
		exec {actual_fd}>&-
		IFS="$OLD_IFS"
	else
		echo "Missing file $EXPECTED_TEXT or $EXPECTED_EVAL or $EXPECTED_REGEX or $EXPECTED_TEXT_UNAME or $EXPECTED_EVAL_UNAME or $EXPECTED_REGEX_UNAME"
		diff -U4 /dev/null "$ACTUAL"
		set -x
		return 1
	fi

	cmp -- "$EXPECTED" "$ACTUAL"
	CMP=$?
	[ $CMP -ne 0 ] && diff -U4 "$EXPECTED" "$ACTUAL"
	set -x
	return $CMP
}

function make_fifo() {
	rm -f "$TESTDIR/$NAME.$1.fifo" || exit $TEST_EX_FAIL
	mkfifo "$TESTDIR/$NAME.$1.fifo" || exit $TEST_EX_FAIL
	echo "$TESTDIR/$NAME.$1.fifo"
}

function __check_variables() {
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
	__check_variables "$@" || exit $TEST_EX_FAIL
	set -x
	exit $TEXT_EX_OK
}

function check_variables_eq() {
	set +x
	__check_variables "$@" || exit $TEST_EX_FAIL
	set -x
}

function run_test() {
	set +x
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
	__periodic_cleanup
	if [ $TEST_NO_STDIN -eq 1 ]; then
		preload_setup
		set -x
		"$TEST_EXEC" "$@" <&- 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt"
		RT_RET1=$?
		set +x
		preload_cleanup
	elif [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		set -x
		FIFO="$(make_fifo "extra-out1")"
		[ $? -eq 0 ] || exit $TEST_EX_FAIL
		set +x
		preload_setup
		set -x
		DTEE_TEST_EXTRA_OUTPUT_FD=3 "$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt" 3>"$FIFO" &
		PID=$!

		cat <"$FIFO" >"$TESTDIR/$NAME.extra-out.txt"

		wait $PID
		RT_RET1=$?
		set +x
		preload_cleanup
	elif [ -n "$TEST_ALT_STDOUT" ]; then
		preload_setup
		set -x
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TEST_ALT_STDOUT" 2>"$TESTDIR/$NAME.err.txt"
		RT_RET1=$?
		set +x
		preload_cleanup
	elif [ -n "$TEST_ALT_STDERR" ]; then
		preload_setup
		set -x
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.out.txt" 2>"$TEST_ALT_STDERR"
		RT_RET1=$?
		set +x
		preload_cleanup
	else
		preload_setup
		set -x
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt"
		RT_RET1=$?
		set +x
		preload_cleanup
	fi
	__periodic_cleanup

	if [ -z "$TEST_ALT_STDOUT" ]; then
		cmp_files "out"
		CMP_OUT=$?
		set +x
	fi

	if [ -z "$TEST_ALT_STDERR" ]; then
		cmp_files "err"
		CMP_ERR=$?
		set +x
	fi

	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		cmp_files "extra-out"
		CMP_EXTRA_OUT1=$?
		set +x
	fi

	# Only run these once because combined output is meaningless
	if [ -n "$TEST_ALT_STDOUT" ]; then
		check_variables_eq CMP_ERR 0
		return $RT_RET1
	elif [ -n "$TEST_ALT_STDERR" ]; then
		check_variables_eq CMP_OUT 0
		return $RT_RET1
	fi

	declare -f test_cleanup >/dev/null && test_cleanup
	declare -f test_prepare >/dev/null && test_prepare

	rm -f "$TESTDIR/$NAME.com.txt" "$TESTDIR/$NAME.extra-out.txt"
	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		rm -f "$TESTDIR/$NAME.extra-out.txt"
	fi
	__periodic_cleanup
	if [ $TEST_NO_STDIN -eq 1 ]; then
		preload_setup
		set -x
		"$TEST_EXEC" "$@" <&- 1>"$TESTDIR/$NAME.com.txt" 2>&1
		RT_RET2=$?
		set +x
		preload_cleanup
	elif [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		set -x
		FIFO="$(make_fifo "extra-out2")"
		[ $? -eq 0 ] || exit $TEST_EX_FAIL
		set +x
		preload_setup
		set -x
		DTEE_TEST_EXTRA_OUTPUT_FD=3 "$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.com.txt" 2>&1 3>"$FIFO" &
		PID=$!

		cat <"$FIFO" >"$TESTDIR/$NAME.extra-out.txt"

		wait $PID
		RT_RET2=$?
		set +x
		preload_cleanup
	else
		preload_setup
		set -x
		"$TEST_EXEC" "$@" <"$STDIN_FILE" 1>"$TESTDIR/$NAME.com.txt" 2>&1
		RT_RET2=$?
		set +x
		preload_cleanup
	fi
	__periodic_cleanup

	cmp_files "com"
	CMP_COM=$?
	set +x

	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		cmp_files "extra-out"
		CMP_EXTRA_OUT2=$?
		set +x
	fi

	declare -f test_cleanup >/dev/null && test_cleanup

	echo RT_RET2 $RT_RET2
	check_variables_eq RT_RET1 $RT_RET2 CMP_OUT 0 CMP_ERR 0 CMP_COM 0
	if [ $TEST_EXTRA_OUTPUT -eq 1 ]; then
		check_variables_eq CMP_EXTRA_OUT1 0 CMP_EXTRA_OUT2 0
	fi

	return $RT_RET1
}

function run_with_preload() {
	set +x
	preload_setup
	set -x
	"$@"
	RWP_RET=$?
	set +x
	preload_cleanup
	set -x
	return $RWP_RET
}

function eval_ret() {
	set +x
	EV_OUTPUT="$("$@")"
	EV_RET=$?
	eval $EV_OUTPUT
	set -x
	return $EV_RET
}

set -x
