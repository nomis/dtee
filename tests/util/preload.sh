EARLY_TEST_LD_PRELOAD=()
COMMON_TEST_LD_PRELOAD=(test-execvp-fd-check test-fork-sigchld-check test-fake-strerror test-fake-strsignal)

UNAME="$(uname)"

case "$UNAME" in
	Darwin)
		SHLIB_PREFIX="./util/lib"
		SHLIB_EXT=.dylib
		;;
	CYGWIN_*)
		SHLIB_PREFIX="./util/cyg"
		SHLIB_EXT=.dll

		# Windows can't find DLLs unless they're in the executable's
		# directory or the PATH. This causes preloaded libraries to
		# fail to find their dependencies, so specify all of them here.
		EARLY_TEST_LD_PRELOAD=(test-is-dtee test-allow-n-times test-fd-unix-socket ${EARLY_TEST_LD_PRELOAD[*]})
		;;
	*)
		SHLIB_PREFIX="./util/lib"
		SHLIB_EXT=.so
		;;
esac

function no_ld_preload() {
	set +x
	NEW_ARRAY=()
	count=${#COMMON_TEST_LD_PRELOAD[@]}
	for ((i = 0; i < count; i++)); do
		if [ "${COMMON_TEST_LD_PRELOAD[i]}" != "$1" ] ; then
			NEW_ARRAY+=(${COMMON_TEST_LD_PRELOAD[i]})
		fi
	done
	COMMON_TEST_LD_PRELOAD=("${NEW_ARRAY[@]}")
	set -x
}

function __get_ld_preload() {
	case "$UNAME" in
		Darwin)
			echo "$DYLD_INSERT_LIBRARIES"
			;;

		*)
			echo "$LD_PRELOAD"
			;;
	esac
}

function __set_ld_preload() {
	case "$UNAME" in
		Darwin)
			if [ -n "$1" ]; then
				set -x
				export DYLD_INSERT_LIBRARIES="$1"
				set +x
			else
				set -x
				unset DYLD_INSERT_LIBRARIES
				set +x
			fi
			;;

		*)
			if [ -n "$1" ]; then
				set -x
				export LD_PRELOAD="$1"
				set +x
			else
				set -x
				unset LD_PRELOAD
				set +x
			fi
			;;
	esac
}

function __build_ld_preload() {
	LD_PRELOAD_STR="$(__get_ld_preload)"

	count=${#EARLY_TEST_LD_PRELOAD[@]}
	for ((i = 0; i < count; i++)); do
		if [ -n "$LD_PRELOAD_STR" ]; then
			LD_PRELOAD_STR="$LD_PRELOAD_STR:"
		fi
		LD_PRELOAD_STR="${LD_PRELOAD_STR}${SHLIB_PREFIX}${EARLY_TEST_LD_PRELOAD[i]}${SHLIB_EXT}"
	done

	count=${#TEST_LD_PRELOAD[@]}
	for ((i = 0; i < count; i++)); do
		if [ -n "$LD_PRELOAD_STR" ]; then
			LD_PRELOAD_STR="$LD_PRELOAD_STR:"
		fi
		LD_PRELOAD_STR="${LD_PRELOAD_STR}${SHLIB_PREFIX}${TEST_LD_PRELOAD[i]}${SHLIB_EXT}"
	done

	count=${#COMMON_TEST_LD_PRELOAD[@]}
	for ((i = 0; i < count; i++)); do
		if [ -n "$LD_PRELOAD_STR" ]; then
			LD_PRELOAD_STR="$LD_PRELOAD_STR:"
		fi
		LD_PRELOAD_STR="${LD_PRELOAD_STR}${SHLIB_PREFIX}${COMMON_TEST_LD_PRELOAD[i]}${SHLIB_EXT}"
	done

	echo "$LD_PRELOAD_STR"
}

function preload_setup() {
	OLD_LD_PRELOAD="$(__get_ld_preload)"
	NEW_LD_PRELOAD="$(__build_ld_preload)"
	__set_ld_preload "$NEW_LD_PRELOAD"
}

function preload_cleanup() {
	__set_ld_preload "$OLD_LD_PRELOAD"
}
