. "$(dirname "$0")"/../util/common.sh

rm -f command_not_executable
(umask 0111; touch command_not_executable; chmod a-x command_not_executable)

run_test "./command_not_executable"
RET=$?

variables_must_eq RET $EX_NOINPUT
