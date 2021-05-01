. "$(dirname "$0")"/util/common.sh

run_test ./util/test-sendfile "$(dirname "$0")/$NAME.in.txt" STDOUT_FILENO
RET=$?

variables_must_eq RET $EXIT_SUCCESS
