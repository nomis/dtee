#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	struct timespec ts = { 0, 10 * 1000 * 1000 }; // 10ms
	struct stat canary_stat = { .st_size = 0 };

	if (argc != 2) {
		return EX_USAGE;
	}

	const char *canary_file = argv[1];

	// Wait for the signal to close stdin (it's a fifo so the writer must have opened it first)
	alarm(10);
	while (stat(canary_file, &canary_stat) != 0) {
		nanosleep(&ts, NULL);
	}

	// Close stdin so that the other side of the pipe receives SIGPIPE or write failures
	close(STDIN_FILENO);

	// Delete the file to signal that we have closed stdin
	unlink(canary_file);
	return EXIT_SUCCESS;
}
