#!/bin/bash
. "$(dirname "$0")"/util/common.sh

# Provide no arguments, get exit code 64 and usage information (excluding cron option)
FIFO=$(make_fifo "out")
./cronty >"$FIFO" &
PID=$!

USAGE=0
CRON=0
while read -r line; do
	echo LINE "$line"
	case "$line" in
	"Usage: ./cronty "*) USAGE=1 ;;
	*"operate in cron mode"*) CRON=1 ;;
	esac
done <"$FIFO"

wait $PID
RET=$?

variables_must_eq RET $EX_USAGE \
	USAGE 1 \
	CRON 0
