#!/bin/bash
set -vx
. "$(dirname "$0")"/util/common.sh

# Ask for help, get exit code 0 and usage information (including cron option)
coproc { ./dtee --help; }
PID=$!

USAGE=0
CRON=0
while read -r line; do
	echo LINE "$line"
	case "$line" in
	"Usage: ./dtee "*) USAGE=1 ;;
	*"operate in cron mode"*) CRON=1 ;;
	esac
done <&$COPROC

wait $PID
RET=$?

variables_must_eq RET 0 \
	USAGE 1 \
	CRON 1
