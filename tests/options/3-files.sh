. "$(dirname -- "$0")"/../util/common.sh

# Numbers are out of order to check they're not being sorted
run_test \
	"-o" "stdout_append2" \
	"-e" "stderr_append3" \
	"-c" "combined_append3" \
	"-O" "stdout_overwrite3" \
	"-E" "stderr_overwrite1" \
	"-C" "combined_overwrite2" \
	\
	"-o" "stdout_append3" \
	"-e" "stderr_append2" \
	"-c" "combined_append1" \
	"-O" "stdout_overwrite1" \
	"-E" "stderr_overwrite3" \
	"-C" "combined_overwrite1" \
	\
	"-o" "stdout_append1" \
	"-e" "stderr_append1" \
	"-c" "combined_append2" \
	"-O" "stdout_overwrite2" \
	"-E" "stderr_overwrite2" \
	"-C" "combined_overwrite3" \
	"--debug-options"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
