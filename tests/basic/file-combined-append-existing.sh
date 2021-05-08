. "$(dirname -- "$0")"/../util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.com-append.txt"
	echo Existing line 1 >"$TESTDIR/$NAME.file.com-append.txt"
	echo Existing line 2 >>"$TESTDIR/$NAME.file.com-append.txt"
	echo Existing line 3 >>"$TESTDIR/$NAME.file.com-append.txt"
}

run_test -c "$TESTDIR/$NAME.file.com-append.txt" "$RUN"
RET=$?

cmp_files "file.com-append"
CMP_COM_A=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_COM_A 0
