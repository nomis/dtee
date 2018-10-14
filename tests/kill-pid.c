#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int signum;

	if (argc != 2) {
		return EXIT_FAILURE;
	}

	signum = strtol(argv[1], NULL, 10);

	printf("Killing current process with signal %d\n", signum);
	fflush(stdout);

	kill(getpid(), signum);
	return EXIT_SUCCESS;
}
