#define __sigaction14 __dtee__hide____sigaction14
#define sigaction __dtee__hide__sigaction

#include <dlfcn.h>
#include <errno.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#undef sigaction
#undef __sigaction14

typedef struct __dtee__hide__sigaction dtee_sigaction_t;

#include "is-dtee.h"
#include "dtee-fcn.h"

TEST_FCN_DECL(int, sigaction, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact));

static int dtee_test_sigaction_restart_already_set(int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact) {
	int (*next_sigaction)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(sigaction);

	if (act == NULL && oldact != NULL) {
		int ret = (*next_sigaction)(signum, act, oldact);
		if (ret == -1) {
			abort();
		}

		// Set SA_RESTART if it's not already set
		if ((oldact->sa_flags & SA_RESTART) == 0) {
			oldact->sa_flags |= SA_RESTART;

			errno = 0;
			ret = (*next_sigaction)(signum, oldact, NULL);
			if (ret == -1) {
				abort();
			}
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
			next_sigaction = dtee_test_sigaction_restart_already_set;
		}

		active = false;
	}

	return (*next_sigaction)(signum, act, oldact);
}

#if defined(__NetBSD__)
static int dtee_test___sigaction14_restart_already_set(int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact) {
	int (*next___sigaction14)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(__sigaction14);

	if (act == NULL && oldact != NULL) {
		int ret = (*next___sigaction14)(signum, act, oldact);
		if (ret == -1) {
			abort();
		}

		// Set SA_RESTART if it's not already set
		if ((oldact->sa_flags & SA_RESTART) == 0) {
			oldact->sa_flags |= SA_RESTART;

			errno = 0;
			ret = (*next___sigaction14)(signum, oldact, NULL);
			if (ret == -1) {
				abort();
			}
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
			next___sigaction14 = dtee_test___sigaction14_restart_already_set;
		}

		active = false;
	}

	return (*next___sigaction14)(signum, act, oldact);
}
#endif
