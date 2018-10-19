#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

run_test \
	"-o" "stdout_append1" \
	"-e" "stderr_append1" \
	"-c" "combined_append1" \
	"-O" "stdout_overwrite1" \
	"-E" "stderr_overwrite1" \
	"-C" "combined_overwrite1" \
	\
	"-o" "stdout_append2" \
	"-e" "stderr_append2" \
	"-c" "combined_append2" \
	"-O" "stdout_overwrite2" \
	"-E" "stderr_overwrite2" \
	"-C" "combined_overwrite2" \
	"--debug-options"
RET=$?

variables_must_eq RET $EXIT_SUCCESS
