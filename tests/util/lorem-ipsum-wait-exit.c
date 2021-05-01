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
	const char *monitor_output = getenv("DTEE_TEST_MONITOR_OUTPUT");
	struct stat output_stat = { .st_size = 0 };

	if (argc != 2) {
		return EX_USAGE;
	}

	err = strtol(argv[1], NULL, 10) != 0;
	assert(monitor_output != NULL);

	int message_len = 0;
	message_len += printf("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor\n");
	fflush(stdout);
	if (err) {
		message_len += fprintf(stderr, "Bacon ipsum dolor amet laborum sirloin bresaola beef ham hock et venison\n");
		fflush(stderr);
	}
	message_len += printf("incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis\n");
	fflush(stdout);
	if (err) {
		message_len += fprintf(stderr, "frankfurter andouille enim jerky pastrami cow. Jerky beef dolor commodo,\n");
		fflush(stderr);
	}
	message_len += printf("nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.\n");
	fflush(stdout);
	if (err) {
		message_len += fprintf(stderr, "pancetta dolore dolore. Consectetur aliqua dolor frankfurter, sed laboris\n");
		fflush(stderr);
	}
	message_len += printf("Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore\n");
	fflush(stdout);
	if (err) {
		message_len += fprintf(stderr, "commodo fatback meatball proident. Do commodo tongue jerky doner. Pork loin\n");
		fflush(stderr);
	}
	message_len += printf("eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt\n");
	fflush(stdout);
	if (err) {
		message_len += fprintf(stderr, "occaecat sunt swine deserunt. Proident turkey dolor est duis qui jowl magna\n");
		fflush(stderr);
	}
	message_len += printf("in culpa qui officia deserunt mollit anim id est laborum.\n");
	fflush(stdout);
	if (err) {
		message_len += fprintf(stderr, "elit ham hock enim boudin brisket laborum t-bone. Drumstick sirloin pig.\n");
		fflush(stderr);
	}

	// Wait for the process to read our output before exiting
	alarm(10);
	while (stat(monitor_output, &output_stat) < 0 || output_stat.st_size != message_len) {
		nanosleep(&ts, NULL);
	}
	return EXIT_SUCCESS;
}
