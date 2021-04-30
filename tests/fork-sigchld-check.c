#include <sys/types.h>
#include <assert.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "is-dtee.h"
#include "dtee-fcn.h"

static void dtee_test_check_sigchld(void) {
	sigset_t set;

	sigemptyset(&set);
	assert(sigprocmask(SIG_SETMASK, NULL, &set) == 0);

	if (!sigismember(&set, SIGCHLD)) {
		struct sigaction act;

		assert(sigaction(SIGCHLD, NULL, &act) == 0);
		if (act.sa_handler == SIG_DFL || act.sa_handler == SIG_IGN) {
			fprintf(stderr, "fork-sigchld-check: SIGCHLD is not blocked and has no signal handler set\n");
			fflush(stderr);
			abort();
		}
	}
}

TEST_FCN_REPL(pid_t, fork, (void)) {
	pid_t (*next_fork)(void) = TEST_FCN_NEXT(fork);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			dtee_test_check_sigchld();
		}

		active = false;
	}

	return (*next_fork)();
}
