#!/bin/bash
set -vx

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

echo RET $RET
echo INFO $INFO
echo COPYRIGHT $COPYRIGHT
echo LICENCE $LICENCE
if [ $RET -eq 0 ] && [ $INFO -eq 1 ] && [ $COPYRIGHT -eq 1 ] && [ $LICENCE -eq 1 ]; then
	exit 0
fi
exit 1
