#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int signum;
	struct timespec ts = { 0, 10 * 1000 * 1000 }; // 10ms
	const char *monitor_stdout = getenv("DTEE_TEST_MONITOR_OUTPUT");
	struct stat stdout_stat = { .st_size = 0 };
	pid_t ppid = getppid();

	if (argc != 2) {
		return EX_USAGE;
	}

	signum = strtol(argv[1], NULL, 10);

	assert(signum != 0);
	assert(monitor_stdout != NULL);

	printf("Killing parent process with signal %d\n", signum);
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
	return EXIT_SUCCESS;
}
