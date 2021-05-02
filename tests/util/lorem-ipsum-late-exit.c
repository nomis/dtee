#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	struct timespec ts = { 0, 10 * 1000 * 1000 }; // 10ms
	pid_t ppid = getppid();
	int lines = 6;

	if (argc > 2) {
		return EX_USAGE;
	} else if (argc == 2) {
		lines = strtoul(argv[1], NULL, 10);
	}

	if (lines-- > 0) {
		printf("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n");
		fflush(stdout);
	}
	if (lines-- > 0) {
		printf("incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis\n");
		fflush(stdout);
	}
	if (lines-- > 0) {
		printf("nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n");
		fflush(stdout);
	}
	if (lines-- > 0) {
		printf("Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore\n");
		fflush(stdout);
	}
	if (lines-- > 0) {
		printf("eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt\n");
		fflush(stdout);
	}
	if (lines-- > 0) {
		printf("in culpa qui officia deserunt mollit anim id est laborum.\n");
		fflush(stdout);
	}

	// Wait for the parent process to exit before us
	// because we don't want our exit status in the output
	alarm(20);
	while (kill(ppid, 0) == 0) {
		nanosleep(&ts, NULL);
	}
	return EXIT_FAILURE;
}
