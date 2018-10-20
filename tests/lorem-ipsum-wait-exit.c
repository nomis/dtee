#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	bool err;
	struct timespec ts = { 0, 10 * 1000 * 1000 }; // 10ms
	const char *monitor_stdout = getenv("DTEE_TEST_MONITOR_OUTPUT");
	struct stat stdout_stat = { .st_size = 0 };

	if (argc != 2) {
		return EX_USAGE;
	}

	err = strtol(argv[1], NULL, 10) != 0;
	assert(monitor_stdout != NULL);

	printf("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n");
	fflush(stdout);
	if (err) {
		fprintf(stderr, "Bacon ipsum dolor amet laborum sirloin bresaola beef ham hock et venison\n");
		fflush(stderr);
	}
	printf("incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis\n");
	fflush(stdout);
	if (err) {
		fprintf(stderr, "frankfurter andouille enim jerky pastrami cow. Jerky beef dolor commodo,\n");
		fflush(stderr);
	}
	printf("nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n");
	fflush(stdout);
	if (err) {
		fprintf(stderr, "pancetta dolore dolore. Consectetur aliqua dolor frankfurter, sed laboris\n");
		fflush(stderr);
	}
	printf("Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore\n");
	fflush(stdout);
	if (err) {
		fprintf(stderr, "commodo fatback meatball proident. Do commodo tongue jerky doner. Pork loin\n");
		fflush(stderr);
	}
	printf("eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt\n");
	fflush(stdout);
	if (err) {
		fprintf(stderr, "occaecat sunt swine deserunt. Proident turkey dolor est duis qui jowl magna\n");
		fflush(stderr);
	}
	printf("in culpa qui officia deserunt mollit anim id est laborum.\n");
	fflush(stdout);
	if (err) {
		fprintf(stderr, "elit ham hock enim boudin brisket laborum t-bone. Drumstick sirloin pig.\n");
		fflush(stderr);
	}

	// Wait for the process to read our output before exiting
	alarm(10);
	while (stat(monitor_stdout, &stdout_stat) < 0 || stdout_stat.st_size == 0) {
		nanosleep(&ts, NULL);
	}
	return EXIT_SUCCESS;
}
