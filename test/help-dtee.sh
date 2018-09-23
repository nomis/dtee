#!/bin/bash
set -vx

# Ask for help, get exit code 0 and usage information (including cron option)
coproc { ./dtee --help; }
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
if [ $RET -eq 0 ] && [ $USAGE -eq 1 ] && [ $CRON -eq 1 ]; then
	exit 0
fi
exit 1
