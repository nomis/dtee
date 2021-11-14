#include <sys/types.h>
#include <sys/wait.h>
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

	pid_t pid = fork();
	if (pid > 0) {
		int wstatus = 0;
		if (waitpid(pid, &wstatus, 0) != -1) {
			if (WIFSIGNALED(wstatus)) {
				if (raise(WTERMSIG(wstatus)) != 0) {
					perror("raise");
					return EX_OSERR;
				}
				return 0x80 + WTERMSIG(wstatus);
			} else if (WIFEXITED(wstatus)) {
				return WEXITSTATUS(wstatus);
			}
			return EX_SOFTWARE;
		} else {
			perror("waitpid");
			return EX_OSERR;
		}
	} else if (pid == 0) {
		execvp(argv[1], &argv[1]);
		return EX_UNAVAILABLE;
	}
	perror("fork");
	return EX_OSERR;
}
