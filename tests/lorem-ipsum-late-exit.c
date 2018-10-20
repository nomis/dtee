#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
	struct timespec ts = { 0, 10 * 1000 * 1000 }; // 10ms
	pid_t ppid = getppid();

	printf("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n");
	fflush(stdout);
	printf("incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis\n");
	fflush(stdout);
	printf("nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n");
	fflush(stdout);
	printf("Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore\n");
	fflush(stdout);
	printf("eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt\n");
	fflush(stdout);
	printf("in culpa qui officia deserunt mollit anim id est laborum.\n");
	fflush(stdout);

	// Wait for the parent process to exit before us
	// because we don't want our exit status in the output
	alarm(10);
	while (kill(ppid, 0) == 0) {
		nanosleep(&ts, NULL);
	}
	return EXIT_FAILURE;
}
