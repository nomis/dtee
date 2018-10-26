#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <time.h>
#include <unistd.h>

#include "is-fd-unix-socket.h"

static void std_report(FILE* output, int fd, const char *name, const char *message) {
	errno = 0;
	ssize_t wlen = write(fd, message, strlen(message));
	int werrno = errno;
	struct sockaddr_un addr;

	fprintf(output, "fd %s write=%zd error=\"%s\"", name, wlen, strerror(werrno));
	if (dtee_test_is_fd_unix_socket(fd, &addr)) {
		char path[sizeof(addr.sun_path) + 1] = { 0 };

		memcpy(path, &addr.sun_path, sizeof(addr.sun_path));
		fprintf(output, " sockname=%s", path);
		fprintf(output, "\n");
	}
	fflush(output);
}

int main(int argc, char *argv[]) {
	int signum;
	struct timespec ts = { 0, 10 * 1000 * 1000 }; // 10ms
	const char *monitor_stdout = getenv("DTEE_TEST_MONITOR_OUTPUT");
	struct stat stdout_stat = { .st_size = 0 };
	const char *extra_stdout = getenv("DTEE_TEST_EXTRA_OUTPUT_FD");
	FILE *extra_output = NULL;
	pid_t ppid = getppid();

	if (argc != 2) {
		return EX_USAGE;
	}

	signum = strtol(argv[1], NULL, 10);

	assert(signum != 0);
	assert(monitor_stdout != NULL);

	if (extra_stdout != NULL) {
		extra_output = fdopen(strtoul(extra_stdout, NULL, 10), "w");
		if (extra_output == NULL) {
			perror("fdopen");
			return EXIT_FAILURE;
		}
	}

	printf("Killing parent process\n");
	fflush(stdout);

	// Wait for the process to read our output before killing it
	alarm(10);
	while (stat(monitor_stdout, &stdout_stat) < 0 || stdout_stat.st_size == 0) {
		nanosleep(&ts, NULL);
	}

	kill(ppid, signum);

	// Wait for the process to exit after handling the signal
	alarm(10);
	while (kill(ppid, 0) == 0) {
		nanosleep(&ts, NULL);
	}

	if (extra_output != NULL) {
		std_report(extra_output, STDOUT_FILENO, "STDOUT_FILENO", "O");
		std_report(extra_output, STDERR_FILENO, "STDERR_FILENO", "E");
	}

	return EXIT_SUCCESS;
}
