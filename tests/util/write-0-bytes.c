#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int fd;

	if (argc != 4) {
		return EX_USAGE;
	}

	if (!strcmp(argv[1], "STDOUT_FILENO")) {
		fd = STDOUT_FILENO;
	} else if (!strcmp(argv[1], "STDERR_FILENO")) {
		fd = STDERR_FILENO;
	} else {
		abort();
	}

	bool quiet = atoi(argv[3]);

	if (!quiet) {
		const char *before = "Before\n";
		int len = strlen(before);
		if (write(fd, before, strlen(before)) != len) {
			return EX_IOERR;
		}
	}

	if (write(fd, "", 0) != 0) {
		return EX_IOERR;
	}

	if (!quiet) {
		const char *after = "After\n";
		int len = strlen(after);
		if (write(fd, after, strlen(after)) != len) {
			return EX_IOERR;
		}
	}

	// Alter exit status in cron mode so that it will output the message
	return strtol(argv[2], NULL, 10) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
