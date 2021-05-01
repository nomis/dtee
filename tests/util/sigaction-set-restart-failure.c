#define __sigaction14 __dtee__hide____sigaction14
#define sigaction __dtee__hide__sigaction

#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>

#undef sigaction
#undef __sigaction14

typedef struct __dtee__hide__sigaction dtee_sigaction_t;

#include "is-dtee.h"
#include "dtee-fcn.h"

TEST_FCN_DECL(int, sigaction, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact));

static int dtee_test_sigaction_set_restart_failure(int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact) {
	int (*next_sigaction)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(sigaction);

	if (act != NULL) {
		if (act->sa_flags & SA_RESTART) {
			errno = EINVAL;
			return -1;
		}
	}

	return (*next_sigaction)(signum, act, oldact);
}

TEST_FCN_REPL(int, sigaction, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact)) {
	int (*next_sigaction)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(sigaction);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_sigaction = dtee_test_sigaction_set_restart_failure;
		}

		active = false;
	}

	return (*next_sigaction)(signum, act, oldact);
}

#if defined(__NetBSD__)
static int dtee_test___sigaction14_set_restart_failure(int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact) {
	int (*next___sigaction14)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(__sigaction14);

	if (act != NULL) {
		if (act->sa_flags & SA_RESTART) {
			errno = EINVAL;
			return -1;
		}
	}

	return (*next___sigaction14)(signum, act, oldact);
}

TEST_FCN_REPL(int, __sigaction14, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact)) {
	int (*next___sigaction14)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(__sigaction14);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next___sigaction14 = dtee_test___sigaction14_set_restart_failure;
		}

		active = false;
	}

	return (*next___sigaction14)(signum, act, oldact);
}
#endif
