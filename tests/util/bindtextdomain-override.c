#define libintl_bindtextdomain __dtee__hide__libintl_bindtextdomain
#define bindtextdomain __dtee__hide__bindtextdomain

#include <dlfcn.h>
#include <libintl.h>
#include <stdbool.h>
#include <stddef.h>

#undef bindtextdomain
#undef libintl_bindtextdomain

#include "is-dtee.h"
#include "dtee-fcn.h"

TEST_FCN_DECL(char*, bindtextdomain, (const char *domainname, const char *dirname));

static char * dtee_test_override_bindtextdomain(const char *domainname, const char *dirname __attribute__((unused))) {
	char* (*next_bindtextdomain)(const char *, const char *) = TEST_FCN_NEXT(bindtextdomain);
	return (*next_bindtextdomain)(domainname, GETTEXT_LOCALEDIR);
}

TEST_FCN_REPL(char*, bindtextdomain, (const char *domainname, const char *dirname)) {
	char* (*next_bindtextdomain)(const char *, const char *) = TEST_FCN_NEXT(bindtextdomain);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_bindtextdomain = dtee_test_override_bindtextdomain;
		}

		active = false;
	}

	return (*next_bindtextdomain)(domainname, dirname);
}

TEST_FCN_DECL(char*, libintl_bindtextdomain, (const char *domainname, const char *dirname));

static char * dtee_test_override_libintl_bindtextdomain(const char *domainname, const char *dirname __attribute__((unused))) {
	char* (*next_libintl_bindtextdomain)(const char *, const char *) = TEST_FCN_NEXT(libintl_bindtextdomain);
	return (*next_libintl_bindtextdomain)(domainname, GETTEXT_LOCALEDIR);
}

TEST_FCN_REPL(char*, libintl_bindtextdomain, (const char *domainname, const char *dirname)) {
	char* (*next_libintl_bindtextdomain)(const char *, const char *) = TEST_FCN_NEXT(libintl_bindtextdomain);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_libintl_bindtextdomain = dtee_test_override_libintl_bindtextdomain;
		}

		active = false;
	}

	return (*next_libintl_bindtextdomain)(domainname, dirname);
}
