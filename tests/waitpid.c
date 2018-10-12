#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <sysexits.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("%s COMMAND ARGV0 [ARG]...\n", argv[0]);
		return EX_USAGE;
	}

	int null = open("/dev/null", O_RDWR);
	if (null < 0) {
		perror("open");
		return EXIT_FAILURE;
	}

	pid_t pid = fork();
	if (pid > 0) {
		int wstatus = 0;
		if (waitpid(pid, &wstatus, 0) != -1) {
			printf("WIFEXITED=%d\n", WIFEXITED(wstatus) ? 1 : 0);
			printf("WEXITSTATUS=%d\n", WEXITSTATUS(wstatus));
			printf("WIFSIGNALED=%d\n", WIFSIGNALED(wstatus) ? 1 : 0);
			printf("WTERMSIG=%d\n", WTERMSIG(wstatus));
#ifdef WCOREDUMP
			printf("WCOREDUMP=%d\n", WCOREDUMP(wstatus) ? 1 : 0);
#endif
			return EXIT_SUCCESS;
		}
	} else if (pid == 0) {
		dup2(null, STDIN_FILENO);
		dup2(null, STDOUT_FILENO);
		dup2(null, STDERR_FILENO);
		close(null);

		execvp(argv[1], &argv[2]);
		abort();
	}
	return EXIT_FAILURE;
}
