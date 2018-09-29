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

function run_test() {
	rm -f "$TESTDIR/$NAME.out" "$TESTDIR/$NAME.err"
	./dtee "$@" 1>"$TESTDIR/$NAME.out" 2>"$TESTDIR/$NAME.err"
	RET1=$?

	cmp "$TESTDIR/$NAME.out" "${0/.sh/.out}"
	CMP_OUT=$?
	[ $CMP_OUT -ne 0 ] && diff -U4 "${0/.sh/.out}" "$TESTDIR/$NAME.out"

	cmp "$TESTDIR/$NAME.err" "${0/.sh/.err}"
	CMP_ERR=$?
	[ $CMP_ERR -ne 0 ] && diff -U4 "${0/.sh/.err}" "$TESTDIR/$NAME.err"

	rm -f "$TESTDIR/$NAME.com"
	./dtee "$@" 1>"$TESTDIR/$NAME.com" 2>&1
	RET2=$?

	cmp "$TESTDIR/$NAME.com" "${0/.sh/.com}"
	CMP_COM=$?
	[ $CMP_COM -ne 0 ] && diff -U4 "${0/.sh/.com}" "$TESTDIR/$NAME.com"

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
	rm -f "$TESTDIR/$NAME.out" "$TESTDIR/$NAME.err"
	./dtee "$@" 1>"$TESTDIR/$NAME.out" 2>"$TESTDIR/$NAME.err"
	RET=$?

	cmp "$TESTDIR/$NAME.out" "${0/.sh/.out}"
	CMP_OUT=$?
	[ $CMP_OUT -ne 0 ] && diff -U4 "${0/.sh/.out}" "$TESTDIR/$NAME.out"

	cmp "$TESTDIR/$NAME.err" "${0/.sh/.err}"
	CMP_ERR=$?
	[ $CMP_ERR -ne 0 ] && diff -U4 "${0/.sh/.err}" "$TESTDIR/$NAME.err"

	echo RET $RET
	echo CMP_OUT $CMP_OUT
	echo CMP_ERR $CMP_ERR
	if [ $CMP_OUT -ne 0 ] || [ $CMP_ERR -ne 0 ]; then
	        exit 1
	fi

	return $RET
}
