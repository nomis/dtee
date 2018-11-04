#define __sigaction14 __dtee__hide____sigaction14
#define sigaction __dtee__hide__sigaction

#include <dlfcn.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>

#undef sigaction
#undef __sigaction14

typedef struct __dtee__hide__sigaction dtee_sigaction_t;

#include "is-dtee.h"

static int dtee_test_sigaction_no_restart(int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact) {
	int (*next_sigaction)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = dlsym(RTLD_NEXT, "sigaction");
	dtee_sigaction_t newact;

	if (act != NULL) {
		newact = *act;
		newact.sa_flags &= ~SA_RESTART;
		act = &newact;
	}

	return (*next_sigaction)(signum, act, oldact);
}

int sigaction(int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact) {
	int (*next_sigaction)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = dlsym(RTLD_NEXT, "sigaction");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_sigaction = dtee_test_sigaction_no_restart;
		}

		active = false;
	}

	return (*next_sigaction)(signum, act, oldact);
}

#if defined(__NetBSD__)
static int dtee_test___sigaction14_no_restart(int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact) {
	int (*next___sigaction14)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = dlsym(RTLD_NEXT, "__sigaction14");
	dtee_sigaction_t newact;

	if (act != NULL) {
		newact = *act;
		newact.sa_flags &= ~SA_RESTART;
		act = &newact;
	}

	return (*next___sigaction14)(signum, act, oldact);
}

int __sigaction14(int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact) {
	int (*next___sigaction14)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = dlsym(RTLD_NEXT, "__sigaction14");
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next___sigaction14 = dtee_test___sigaction14_no_restart;
		}

		active = false;
	}

	return (*next___sigaction14)(signum, act, oldact);
}
#endif
