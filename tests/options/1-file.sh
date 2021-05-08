. "$(dirname -- "$0")"/../util/common.sh

run_test \
	"-o" "stdout_append" \
	"-e" "stderr_append" \
	"-c" "combined_append" \
	"-O" "stdout_overwrite" \
	"-E" "stderr_overwrite" \
	"-C" "combined_overwrite" \
	"--debug-options"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
