#include <errno.h>
#include <dlfcn.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "is-dtee.h"

static char *dtee_test_fake_strsignal(int sig) {
	char *(*next_strsignal)(int) = dlsym(RTLD_NEXT, "strsignal");

	switch (sig) {
#define STRSIGNAL_FOR(__sig) \
	case __sig: \
		return #__sig;

#ifdef SIGCHLD
	STRSIGNAL_FOR(SIGCHLD)
#endif
#ifdef SIGHUP
	STRSIGNAL_FOR(SIGHUP)
#endif
#ifdef SIGINT
	STRSIGNAL_FOR(SIGINT)
#endif
#ifdef SIGKILL
	STRSIGNAL_FOR(SIGKILL)
#endif
#ifdef SIGPIPE
	STRSIGNAL_FOR(SIGPIPE)
#endif
#ifdef SIGTERM
	STRSIGNAL_FOR(SIGTERM)
#endif
#ifdef SIGQUIT
	STRSIGNAL_FOR(SIGQUIT)
#endif

#undef STRSIGNAL_FOR

	default:
		return (*next_strsignal)(sig);
	}
}

char *strsignal(int sig) {
	char *(*next_strsignal)(int) = dlsym(RTLD_NEXT, "strsignal");

	if (dtee_test_is_dtee() || dtee_test_is_ppid_dtee() || dtee_test_is_dtee_test()) {
		next_strsignal = dtee_test_fake_strsignal;
	}

	return (*next_strsignal)(sig);
}
