TESTDIR="dtee@test"
mkdir -p "$TESTDIR"

NAME="$(basename "$0")"
RUN="${0/.sh/.run}"

function run_test() {
	rm -f "$TESTDIR/$NAME.out" "$TESTDIR/$NAME.err"
	./dtee "$@" 1>"$TESTDIR/$NAME.out" 2>"$TESTDIR/$NAME.err"
	RET1=$?

	./dtee "$@" 1>"$TESTDIR/$NAME.com" 2>&1
	RET2=$?

	cmp "$TESTDIR/$NAME.out" "${0/.sh/.out}"
	CMP_OUT=$?
	[ $CMP_OUT -ne 0 ] && diff -U4 "$TESTDIR/$NAME.out" "${0/.sh/.out}"

	cmp "$TESTDIR/$NAME.err" "${0/.sh/.err}"
	CMP_ERR=$?
	[ $CMP_ERR -ne 0 ] && diff -U4 "$TESTDIR/$NAME.err" "${0/.sh/.err}"

	cmp "$TESTDIR/$NAME.com" "${0/.sh/.com}"
	CMP_COM=$?
	[ $CMP_COM -ne 0 ] && diff -U4 "$TESTDIR/$NAME.com" "${0/.sh/.com}"

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
