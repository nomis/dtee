. "$(dirname "$0")"/util/common.sh

function test_prepare() {
	rm -f "$TESTDIR/$NAME.file.out-append.txt" "$TESTDIR/$NAME.file.err-append.txt" "$TESTDIR/$NAME.file.com-append.txt"
	rm -f "$TESTDIR/$NAME.file.out-overwrite.txt" "$TESTDIR/$NAME.file.err-overwrite.txt" "$TESTDIR/$NAME.file.com-overwrite.txt"
}

TEST_LD_PRELOAD="./libtest-mkdtemp-consistent.so:./libtest-fake-getpid.so:./libtest-fake-getuid.so"
run_test \
	-o "$TESTDIR/$NAME.file.out-append.txt" -O "$TESTDIR/$NAME.file.out-overwrite.txt" \
	-e "$TESTDIR/$NAME.file.err-append.txt" -E "$TESTDIR/$NAME.file.err-overwrite.txt" \
	-c "$TESTDIR/$NAME.file.com-append.txt" -C "$TESTDIR/$NAME.file.com-overwrite.txt" \
	./test-report 'Hello World!' ETAOIN SHRDLU
RET=$?

cmp_files "file.out-append"
CMP_OUT_A=$?

cmp_files "file.out-overwrite"
CMP_OUT_O=$?

cmp_files "file.err-append"
CMP_ERR_A=$?

cmp_files "file.err-overwrite"
CMP_ERR_O=$?

cmp_files "file.com-append"
CMP_COM_A=$?

cmp_files "file.com-overwrite"
CMP_COM_O=$?

variables_must_eq RET $EXIT_SUCCESS \
	CMP_OUT_A 0 \
	CMP_OUT_O 0 \
	CMP_ERR_A 0 \
	CMP_ERR_O 0 \
	CMP_COM_A 0 \
	CMP_COM_O 0
