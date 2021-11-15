#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
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
	signal(SIGHUP, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	signal(SIGABRT, SIG_DFL);
	signal(SIGSEGV, SIG_DFL);
	signal(SIGPIPE, SIG_DFL);
	signal(SIGALRM, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGCHLD, SIG_DFL);
	signal(SIGCONT, SIG_DFL);

	execvp(argv[1], &argv[1]);
	perror("execvp");
	return EX_UNAVAILABLE;
}
