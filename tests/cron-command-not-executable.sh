#!/bin/bash
. "$(dirname "$0")"/util/common.sh

rm -f cron_command_not_executable
(umask 0111; touch cron_command_not_executable)

run_test "-q" "./cron_command_not_executable"
RET=$?

variables_must_eq RET $EX_NOINPUT
