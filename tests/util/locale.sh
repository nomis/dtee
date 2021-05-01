mkdir -p "util/$BASETESTDIR/locale/$TEST_LANGUAGE/LC_MESSAGES"
ln -sf "../../../../../../i18n/test-${TEST_LANGUAGE}.mo" "util/$BASETESTDIR/locale/$TEST_LANGUAGE/LC_MESSAGES/dtee.mo"

# Find the best possible system language to use when running the test
FOUND_LOCALES=("" "" "" "")
while read -r line; do
	case "$line" in
		C|POSIX)
			;;

		C.utf8|C.UTF8|C.utf-8|C.UTF-8)
			FOUND_LOCALES[3]="$line"
			;;

		"$TEST_LANGUAGE"|"$TEST_LANGUAGE".*)
			FOUND_LOCALES[0]="$line"
			;;

		*.utf8|*.UTF8|*.utf-8|*.UTF-8)
			FOUND_LOCALES[1]="$line"
			;;

		*)
			FOUND_LOCALES[2]="$line"
			;;
	esac
done < <(locale -a)

count=${#FOUND_LOCALES[@]}
for ((i = 0; i < count; i++)); do
	[ ! -z "${FOUND_LOCALES[i]}" ] && FOUND_LOCALE="${FOUND_LOCALES[i]}" && return
done

echo "No locales found. At least one locale other than C or POSIX must be"
echo "present for gettext() to translate messages."
exit 1
