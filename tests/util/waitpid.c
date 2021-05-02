#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdbool.h>
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

	// This is a bit of a mess. It would be easier to write our output to a
	// an alternative file descriptor, then the calling process could set
	// up stdio streams as required.
	const char *env_closed_stdin = getenv("TEST_WAITPID_CLOSED_STDIN");
	const char *env_closed_stdout = getenv("TEST_WAITPID_CLOSED_STDOUT");
	const char *env_closed_stderr = getenv("TEST_WAITPID_CLOSED_STDERR");
	bool closed_stdin = (env_closed_stdin != NULL && strtoul(env_closed_stdin, NULL, 10) == 1);
	bool closed_stdout = (env_closed_stdout != NULL && strtoul(env_closed_stdout, NULL, 10) == 1);
	bool closed_stderr = (env_closed_stderr != NULL && strtoul(env_closed_stderr, NULL, 10) == 1);

	int read_pipefd[2];
	if (pipe(read_pipefd) < 0) {
		perror("pipe");
		return EXIT_FAILURE;
	}
	close(read_pipefd[1]);

	int write_pipefd[2];
	if (pipe(write_pipefd) < 0) {
		perror("pipe");
		return EXIT_FAILURE;
	}
	close(write_pipefd[0]);

	pid_t pid = fork();
	if (pid > 0) {
		close(null);
		close(read_pipefd[0]);
		close(write_pipefd[1]);

		int wstatus = 0;
		alarm(20);
		if (waitpid(pid, &wstatus, 0) != -1) {
			printf("WIFEXITED=%d\n", WIFEXITED(wstatus) ? 1 : 0);
			printf("WEXITSTATUS=%d\n", WEXITSTATUS(wstatus));
			printf("WIFSIGNALED=%d\n", WIFSIGNALED(wstatus) ? 1 : 0);
			printf("WTERMSIG=%d\n", WTERMSIG(wstatus));
#ifdef WCOREDUMP
			printf("WCOREDUMP=%d\n", WCOREDUMP(wstatus) ? 1 : 0);
#endif
			return EXIT_SUCCESS;
		} else {
			perror("waitpid");
		}
	} else if (pid == 0) {
		if (dup2(closed_stdin ? read_pipefd[0] : null, STDIN_FILENO) < 0) {
			abort();
		}
		if (dup2(closed_stdout ? write_pipefd[1] : null, STDOUT_FILENO) < 0) {
			abort();
		}
		if (dup2(closed_stderr ? write_pipefd[1] : null, STDERR_FILENO) < 0) {
			abort();
		}

		close(null);
		close(read_pipefd[0]);
		close(write_pipefd[1]);

		execvp(argv[1], &argv[2]);
		abort();
	}
	return EXIT_FAILURE;
}
