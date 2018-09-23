#!/bin/bash
set -vx

# Provide no arguments, get exit code 1 and usage information (excluding cron option)
coproc { ./cronic; }
PID=$!

USAGE=0
CRON=0
while read -r line; do
	echo LINE "$line"
	case "$line" in
	"Usage:"*) USAGE=1 ;;
	*"operate in cron mode"*) CRON=1 ;;
	esac
done <&$COPROC

wait $PID
RET=$?

echo RET $RET
echo USAGE $USAGE
echo CRON $CRON
if [ $RET -eq 1 ] && [ $USAGE -eq 1 ] && [ $CRON -eq 0 ]; then
	exit 0
fi
exit 1
