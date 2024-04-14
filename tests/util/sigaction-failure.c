#define __sigaction_siginfo __dtee__hide____sigaction_siginfo
#define __sigaction14 __dtee__hide____sigaction14
#define sigaction __dtee__hide__sigaction

#include <dlfcn.h>
#include <errno.h>
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

static int dtee_test_sigaction_failure(int signum __attribute__((unused)), const dtee_sigaction_t *act __attribute__((unused)), dtee_sigaction_t *oldact __attribute__((unused))) {
	errno = EINVAL;
	return -1;
}

TEST_FCN_REPL(int, sigaction, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact)) {
	int (*next_sigaction)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(sigaction);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next_sigaction = dtee_test_sigaction_failure;
		}

		active = false;
	}

	return (*next_sigaction)(signum, act, oldact);
}

#if defined(__NetBSD__)
static int dtee_test___sigaction14_failure(int signum __attribute__((unused)), const dtee_sigaction_t *act __attribute__((unused)), dtee_sigaction_t *oldact __attribute__((unused))) {
	errno = EINVAL;
	return -1;
}

TEST_FCN_REPL(int, __sigaction14, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact)) {
	int (*next___sigaction14)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(__sigaction14);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next___sigaction14 = dtee_test___sigaction14_failure;
		}

		active = false;
	}

	return (*next___sigaction14)(signum, act, oldact);
}

static int dtee_test___sigaction_siginfo_failure(int signum __attribute__((unused)), const dtee_sigaction_t *act __attribute__((unused)), dtee_sigaction_t *oldact __attribute__((unused))) {
	errno = EINVAL;
	return -1;
}

TEST_FCN_REPL(int, __sigaction_siginfo, (int signum, const dtee_sigaction_t *act, dtee_sigaction_t *oldact)) {
	int (*next___sigaction_siginfo)(int, const dtee_sigaction_t *, dtee_sigaction_t *) = TEST_FCN_NEXT(__sigaction_siginfo);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			next___sigaction_siginfo = dtee_test___sigaction_siginfo_failure;
		}

		active = false;
	}

	return (*next___sigaction_siginfo)(signum, act, oldact);
}
#endif
