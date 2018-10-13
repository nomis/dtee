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

function run_test() {
	rm -f "$TESTDIR/$NAME.out.txt" "$TESTDIR/$NAME.err.txt"
	before_test
	./dtee "$@" 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt"
	RET1=$?
	after_test

	cmp "$TESTDIR/$NAME.out.txt" "${0/.sh/.out.txt}"
	CMP_OUT=$?
	[ $CMP_OUT -ne 0 ] && diff -U4 "${0/.sh/.out.txt}" "$TESTDIR/$NAME.out.txt"

	cmp "$TESTDIR/$NAME.err.txt" "${0/.sh/.err.txt}"
	CMP_ERR=$?
	[ $CMP_ERR -ne 0 ] && diff -U4 "${0/.sh/.err.txt}" "$TESTDIR/$NAME.err.txt"

	rm -f "$TESTDIR/$NAME.com.txt"
	before_test
	./dtee "$@" 1>"$TESTDIR/$NAME.com.txt" 2>&1
	RET2=$?
	after_test

	cmp "$TESTDIR/$NAME.com.txt" "${0/.sh/.com.txt}"
	CMP_COM=$?
	[ $CMP_COM -ne 0 ] && diff -U4 "${0/.sh/.com.txt}" "$TESTDIR/$NAME.com.txt"

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

function run_test_once() {
	rm -f "$TESTDIR/$NAME.out.txt" "$TESTDIR/$NAME.err.txt"
	before_test
	./dtee "$@" 1>"$TESTDIR/$NAME.out.txt" 2>"$TESTDIR/$NAME.err.txt"
	RET=$?
	after_test

	cmp "$TESTDIR/$NAME.out.txt" "${0/.sh/.out.txt}"
	CMP_OUT=$?
	[ $CMP_OUT -ne 0 ] && diff -U4 "${0/.sh/.out.txt}" "$TESTDIR/$NAME.out.txt"

	cmp "$TESTDIR/$NAME.err.txt" "${0/.sh/.err.txt}"
	CMP_ERR=$?
	[ $CMP_ERR -ne 0 ] && diff -U4 "${0/.sh/.err.txt}" "$TESTDIR/$NAME.err.txt"

	echo RET $RET
	echo CMP_OUT $CMP_OUT
	echo CMP_ERR $CMP_ERR
	if [ $CMP_OUT -ne 0 ] || [ $CMP_ERR -ne 0 ]; then
	        exit 1
	fi

	return $RET
}
