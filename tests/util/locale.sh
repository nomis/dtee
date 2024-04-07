set +x
mkdir -p "util/$BASETESTDIR/locale/$TEST_LANGUAGE/LC_MESSAGES"
ln -s "../../../../../../i18n/test-${TEST_LANGUAGE}.mo" "util/$BASETESTDIR/locale/$TEST_LANGUAGE/LC_MESSAGES/dtee.mo"

# Find the best possible system language to use when running the test
FOUND_LOCALES=("" "" "" "" "")
while read -r line; do
	case "$line" in
		C|C.*|POSIX)
			;;

		"$TEST_LANGUAGE".utf8|"$TEST_LANGUAGE".UTF8|"$TEST_LANGUAGE".utf-8|"$TEST_LANGUAGE".UTF-8)
			FOUND_LOCALES[0]="$line"
			;;

		"$TEST_LANGUAGE".*)
			FOUND_LOCALES[1]="$line"
			;;

		"$TEST_LANGUAGE")
			FOUND_LOCALES[2]="$line"
			;;

		*.utf8|*.UTF8|*.utf-8|*.UTF-8)
			FOUND_LOCALES[3]="$line"
			;;

		*)
			FOUND_LOCALES[4]="$line"
			;;
	esac
done < <(locale -a)

count=${#FOUND_LOCALES[@]}
for ((i = 0; i < count; i++)); do
	[ ! -z "${FOUND_LOCALES[i]}" ] && FOUND_LOCALE="${FOUND_LOCALES[i]}" && set -x && return
done

echo "No usable locales found. At least one locale other than C, C.* or POSIX must"
echo "be present for gettext() to translate messages."
locale -a
if [ "$CI" = "true" ]; then
	exit $TEST_EX_FAIL
else
	exit $TEST_EX_SKIP
fi
