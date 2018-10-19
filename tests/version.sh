#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Ask for the version, get exit code 0 and version information
coproc { ./dtee --version; }
PID=$!

INFO=0
COPYRIGHT=0
LICENCE=0
while read -r line; do
	echo LINE "$line"
	case "$line" in
	"dtee - run a program with standard output and standard error copied to files") INFO=1 ;;
	"Copyright "*) COPYRIGHT=1 ;;
	"Licence GPLv3+"*) LICENCE=1 ;;
	esac
done <&$COPROC

wait $PID
RET=$?

variables_must_eq RET $EXIT_SUCCESS \
	INFO 1 \
	COPYRIGHT 1 \
	LICENCE 1
