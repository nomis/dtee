#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int signum;
	struct timespec ts = { 0, 100 * 1000 * 1000 };

	if (argc != 2) {
		return EXIT_FAILURE;
	}

	signum = strtol(argv[1], NULL, 10);

	printf("Killing parent process with signal %d\n", signum);
	fflush(stdout);

	// Allow enough time for the process to read our output before killing it
	nanosleep(&ts, NULL);

	kill(getppid(), signum);
	return EXIT_SUCCESS;
}
