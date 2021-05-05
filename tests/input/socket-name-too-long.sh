. "$(dirname "$0")"/../util/common.sh

export TMPDIR="$TMPDIR/AAAAAAAAAAAAAAAAAAAAAAAAA/BBBBBBBBBBBBBBBBBBBBBBBBB/CCCCCCCCCCCCCCCCCCCCCCCCC/DDDDDDDDDDDDDDDDDDDDDDDDD"

function test_prepare() {
	mkdir -p "$TMPDIR"
}

TEST_LD_PRELOAD=(test-fake-getpid test-fake-getuid)
run_test "$RUN"
RET=$?

variables_must_eq RET $EX_UNAVAILABLE
