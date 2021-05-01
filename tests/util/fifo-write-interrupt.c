#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <time.h>
#include <unistd.h>

#define min(a, b) (((a) < (b)) ? (a) : (b))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

int main(int argc, char *argv[]) {
	if (argc != 6 || (strcmp(argv[2], "-i") && strcmp(argv[2], "-q"))) {
		printf("%s COMMAND [-i|-q] FILE1 FIFO FILE2\n", argv[0]);
		return EX_USAGE;
	}

	char *command = argv[1];
	char *extra_arg = argv[2];
	char *file1_name = argv[3];
	char *fifo_name = argv[4];
	char *file2_name = argv[5];

	int fifo_r = open(fifo_name, O_RDONLY | O_CLOEXEC | O_NONBLOCK);
	if (fifo_r < 0) {
		perror("open");
		return EXIT_FAILURE;
	}

	int fifo_w = open(fifo_name, O_WRONLY | O_CLOEXEC | O_NONBLOCK);
	if (fifo_w < 0) {
		perror("open");
		return EXIT_FAILURE;
	}

	int stdin_pipe[2];
	if (pipe(stdin_pipe) < 0) {
		perror("pipe");
		return EXIT_FAILURE;
	}

	// Fill up the fifo so that dtee blocks writing to it
	size_t written = 0;
	ssize_t ret;
	do {
		char buf[BUFSIZ] = { '*' };

		errno = 0;
		ret = write(fifo_w, buf, sizeof(buf));
		if (ret > 0) {
			written += ret;
		}
	} while (ret > 0);
	// Some platforms won't do partial writes to a fifo
	do {
		char buf[1] = { '*' };

		errno = 0;
		ret = write(fifo_w, buf, sizeof(buf));
		if (ret > 0) {
			written += ret;
		}
	} while (ret > 0);

	pid_t pid = fork();
	if (pid > 0) {
		int outcome = EXIT_SUCCESS;

		close(stdin_pipe[0]);

		// Write message to stdin
		const char *message = "Test message\n";
		if (write(stdin_pipe[1], message, strlen(message)) != (ssize_t)strlen(message)) {
			perror("write");
			return EXIT_FAILURE;
		}

		struct timespec ts = { 0, 10 * 1000 * 1000 }; // 10ms
		struct stat file_stat = { .st_size = 0 };

		// Wait for the message to be written to file1
		alarm(10);
		errno = 0;
		while (stat(file1_name, &file_stat) != 0 || file_stat.st_size == 0) {
			nanosleep(&ts, NULL);
			errno = 0;
		}
		nanosleep(&ts, NULL);

		// Signal the process
		kill(pid, SIGINT);
		nanosleep(&ts, NULL);
		kill(pid, SIGSTOP);
		nanosleep(&ts, NULL);
		kill(pid, SIGCONT);
		nanosleep(&ts, NULL);
		kill(pid, SIGWINCH);
		nanosleep(&ts, NULL);
		kill(pid, SIGINT);
		nanosleep(&ts, NULL);

		// Check nothing has been written to file 2
		errno = 0;
		assert(stat(file2_name, &file_stat) == 0);
		assert(errno == 0);
		if (file_stat.st_size != 0) {
			outcome = 2;
		}

		// Unblock the fifo
		alarm(10);
		while (written > 0) {
			char buf[BUFSIZ];

			ret = read(fifo_r, buf, min(written, sizeof(buf)));
			if (ret > 0) {
				written -= ret;
			} else {
				perror("read");
				return EXIT_FAILURE;
			}
		}

		// Wait for the message to be written to file 2
		alarm(10);
		errno = 0;
		while (stat(file2_name, &file_stat) != 0 || file_stat.st_size == 0) {
			nanosleep(&ts, NULL);
			errno = 0;
		}

		if (!strcmp(extra_arg, "-i")) {
			// Stop writing so that "cat" will now exit
			close(stdin_pipe[1]);
		}

		// Wait for the process to exit
		alarm(10);
		int wstatus = 0;
		if (waitpid(pid, &wstatus, 0) != -1) {
			printf("WIFEXITED=%d\n", WIFEXITED(wstatus) ? 1 : 0);
			printf("WEXITSTATUS=%d\n", WEXITSTATUS(wstatus));
			printf("WIFSIGNALED=%d\n", WIFSIGNALED(wstatus) ? 1 : 0);
			printf("WTERMSIG=%d\n", WTERMSIG(wstatus));

			if (!strcmp(extra_arg, "-q")) {
				// Stop writing so that "cat" will now exit
				close(stdin_pipe[1]);
			}

			printf("Reading from fifo...");
			char buf[BUFSIZ] = { 0 };
			ssize_t count = read(fifo_r, buf, sizeof(buf) - 1);
			printf(" %zd bytes:\n%s", count, buf);
			return outcome;
		} else {
			perror("waitpid");
		}
	} else if (pid == 0) {
		close(stdin_pipe[1]);
		if (dup2(stdin_pipe[0], STDIN_FILENO) < 0) {
			abort();
		}

		execlp(command, command, extra_arg,
			"-O", file1_name,
			"-O", fifo_name,
			"-O", file2_name,
			"cat", NULL);
		abort();
	}
	return EXIT_FAILURE;
}
