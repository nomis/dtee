#define __sigaction_siginfo __dtee__hide____sigaction_siginfo
#define __sigaction14 __dtee__hide____sigaction14
#define sigaction __dtee__hide__sigaction

#include <signal.h>
#include <stdbool.h>
#include <stddef.h>

#undef sigaction
#undef __sigaction14
#undef __sigaction_siginfo

typedef struct __dtee__hide__sigaction dtee_sigaction_t;

#include "is-dtee.h"
#include "dtee-fcn.h"

TEST_FCN_DECL(int, sigaction, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact));

static int dtee_test_sigaction_no_restart(int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact) {
	int (*next_sigaction)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(sigaction);
	dtee_sigaction_t newact;

	if (act != NULL) {
		newact = *act;
		newact.sa_flags &= ~SA_RESTART;
		act = &newact;
	}

	return (*next_sigaction)(signum, act, oldact);
}

TEST_FCN_REPL(int, sigaction, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact)) {
	int (*next_sigaction)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(sigaction);
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
	int (*next___sigaction14)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(__sigaction14);
	dtee_sigaction_t newact;

	if (act != NULL) {
		newact = *act;
		newact.sa_flags &= ~SA_RESTART;
		act = &newact;
	}

	return (*next___sigaction14)(signum, act, oldact);
}

TEST_FCN_REPL(int, __sigaction14, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact)) {
	int (*next___sigaction14)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(__sigaction14);
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

static int dtee_test___sigaction_siginfo_no_restart(int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact) {
	int (*next___sigaction_siginfo)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(__sigaction_siginfo);
	dtee_sigaction_t newact;

	if (act != NULL) {
		newact = *act;
		newact.sa_flags &= ~SA_RESTART;
		act = &newact;
	}

	return (*next___sigaction_siginfo)(signum, act, oldact);
}

TEST_FCN_REPL(int, __sigaction_siginfo, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact)) {
	int (*next___sigaction_siginfo)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(__sigaction_siginfo);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next___sigaction_siginfo = dtee_test___sigaction_siginfo_no_restart;
		}

		active = false;
	}

	return (*next___sigaction_siginfo)(signum, act, oldact);
}
#endif
