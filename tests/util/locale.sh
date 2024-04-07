set +x
mkdir -p "util/$BASETESTDIR/locale/$TEST_LANGUAGE/LC_MESSAGES"
ln -s "../../../../../../i18n/test-${TEST_LANGUAGE}.mo" "util/$BASETESTDIR/locale/$TEST_LANGUAGE/LC_MESSAGES/dtee.mo"

# Find the best possible system language to use when running the test
FOUND_LOCALES=("" "" "" "" "" "")
while read -r line; do
	case "$line" in
		C|POSIX)
			;;

		C.utf8|C.UTF8|C.utf-8|C.UTF-8)
			FOUND_LOCALES[5]="$line"
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

echo "No locales found. At least one locale other than C or POSIX must be"
echo "present for gettext() to translate messages."
exit 1
