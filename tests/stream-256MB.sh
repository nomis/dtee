. "$(dirname "$0")"/util/common.sh

BYTES=$((256 * 1024 * 1024))
./dtee ./test-stream-write $BYTES | ./test-stream-read $BYTES
RET=$?

variables_must_eq RET 0
