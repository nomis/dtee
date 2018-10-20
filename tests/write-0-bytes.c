#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	int fd;

	if (argc != 3) {
		return EX_USAGE;
	}

	if (!strcmp(argv[1], "STDOUT_FILENO")) {
		fd = STDOUT_FILENO;
	} else if (!strcmp(argv[1], "STDERR_FILENO")) {
		fd = STDERR_FILENO;
	} else {
		abort();
	}

	const char *before = "Before\n";
	if (write(fd, before, strlen(before)) != (ssize_t)strlen(before)) {
		return EX_IOERR;
	}
	if (write(fd, "", 0) != 0) {
		return EX_IOERR;
	}
	const char *after = "After\n";
	if (write(fd, after, strlen(after)) != (ssize_t)strlen(after)) {
		return EX_IOERR;
	}

	// Alter exit status in cron mode so that it will output the message
	return strtol(argv[2], NULL, 10) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
