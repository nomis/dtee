. "$(dirname "$0")"/util/common.sh

# Ask for the version, get exit code 0 and version information
FIFO=$(make_fifo "out")
./dtee --version >"$FIFO" &
PID=$!

VERSION=0
COPYRIGHT=0
LICENCE=0
EMPTY=0
TRANSLATION=0
CREDITS=0
while read -r line; do
	echo LINE "$line"
	case "$line" in
	"dtee "*.*.*) VERSION=1 ;;
	"Copyright "*) COPYRIGHT=1 ;;
	"Licence GPLv3+"*) LICENCE=1 ;;
	"") EMPTY=1 ;;
	*Translation*) TRANSLATION=1 ;;
	*translator*credits*) CREDITS=1 ;;
	esac
done <"$FIFO"

wait $PID
RET=$?

variables_must_eq RET $EXIT_SUCCESS \
	VERSION 1 \
	COPYRIGHT 1 \
	LICENCE 1 \
	EMPTY 0 \
	TRANSLATION 0 \
	CREDITS 0
