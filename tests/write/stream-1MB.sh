. "$(dirname -- "$0")"/../util/common.sh

BYTES=$((1024 * 1024))
./dtee ./util/test-stream-write $BYTES | ./util/test-stream-read $BYTES
RET=$?

variables_must_eq RET $EXIT_SUCCESS
