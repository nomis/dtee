#ifdef __CYGWIN__
# define __gnu_basename __dtee__hide____gnu_basename
# define basename __dtee__hide__basename

# include <libgen.h>
#endif

#include <dlfcn.h>
#include <libintl.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifdef __CYGWIN__
# undef basename
# undef __gnu_basename
#else
# undef bindtextdomain
# undef libintl_bindtextdomain
#endif

#include "is-dtee.h"
#include "dtee-fcn.h"

#ifdef __CYGWIN__
// The only non-libintl function that dtee calls after bindtextdomain()
// but before calling gettext() is basename(). It is only possible to
// override Cygwin functions, not those in the libintl DLL.

static void dtee_test_bindtextdomain(void) {
	static bool set = false;

	if (!set) {
		bindtextdomain("dtee", GETTEXT_LOCALEDIR);
		set = true;
	}
}

TEST_FCN_DECL(char*, basename, (char *));

TEST_FCN_REPL(char*, basename, (char *path)) {
	char* (*next_basename)(char *) = TEST_FCN_NEXT(basename);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			dtee_test_bindtextdomain();
		}

		active = false;
	}

	return (*next_basename)(path);
}

TEST_FCN_DECL(char*, __gnu_basename, (char *));

TEST_FCN_REPL(char*, __gnu_basename, (char *path)) {
	char* (*next___gnu_basename)(char *) = TEST_FCN_NEXT(__gnu_basename);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			dtee_test_bindtextdomain();
		}

		active = false;
	}

	return (*next___gnu_basename)(path);
}
#else
TEST_FCN_DECL(char*, bindtextdomain, (const char *domainname, const char *dirname));

static char * dtee_test_override_bindtextdomain(const char *domainname, const char *dirname) {
	char* (*next_bindtextdomain)(const char *, const char *) = TEST_FCN_NEXT(bindtextdomain);

	if (!strcmp(domainname, "dtee")) {
		dirname = GETTEXT_LOCALEDIR;
	}

	return (*next_bindtextdomain)(domainname, dirname);
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

static char * dtee_test_override_libintl_bindtextdomain(const char *domainname, const char *dirname) {
	char* (*next_libintl_bindtextdomain)(const char *, const char *) = TEST_FCN_NEXT(libintl_bindtextdomain);

	if (!strcmp(domainname, "dtee")) {
		dirname = GETTEXT_LOCALEDIR;
	}

	return (*next_libintl_bindtextdomain)(domainname, dirname);
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
#endif
