#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <unistd.h>

static bool restore_signal(sigset_t *set, int signum) {
	sigaddset(set, signum);
	return signal(signum, SIG_DFL) != SIG_ERR;
}

int main(int argc, char *argv[]) {
	sigset_t set;

	if (argc < 2) {
		printf("%s COMMAND [ARG]...\n", argv[0]);
		return EX_USAGE;
	}

	// When a shell executes a command in the background it will ignore the
	// SIGINT and SIGQUIT signals. This is then propagated through all fork()
	// and execve() calls. Reset these signal handlers to the default before
	// running the command otherwise tests involving SIGINT and SIGQUIT will not
	// behave as expected. Reset other signal handlers too just in case
	// something else has ignored them.

	sigemptyset(&set);

	assert(restore_signal(&set, SIGHUP));
	assert(restore_signal(&set, SIGINT));
	assert(restore_signal(&set, SIGQUIT));
	assert(restore_signal(&set, SIGABRT));
	assert(restore_signal(&set, SIGSEGV));
	assert(restore_signal(&set, SIGPIPE));
	assert(restore_signal(&set, SIGALRM));
	assert(restore_signal(&set, SIGTERM));
	assert(restore_signal(&set, SIGCHLD));
	assert(restore_signal(&set, SIGCONT));

	// Also unblock the signals!

	if (sigprocmask(SIG_UNBLOCK, &set, NULL) != 0) {
		perror("sigprocmask");
		return EX_OSERR;
	}

	execvp(argv[1], &argv[1]);
	perror("execvp");
	return EX_UNAVAILABLE;
}
