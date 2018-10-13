#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static bool std_report(int fd, const char *name, const char *message) {
	bool ok = true;
	char buf[1] = { 0 };

	errno = 0;
	if (fcntl(fd, F_GETFL) != -1) {
		ssize_t rlen = read(fd, buf, 1);
		ssize_t wlen = write(fd, message, strlen(message));
		ok &= printf("fd %s open read=%zd (%02x) write=%zd\n", name, rlen, buf[0], wlen) > 0;
		ok &= !fflush(stdout);
	} else {
		ok &= printf("fd %s closed errno=%d\n", name, errno) > 0;
		ok &= !fflush(stdout);
	}

	return ok;
}

int main(int argc, char *argv[]) {
	bool ok = true;

	ok &= printf("argc=%d\n", argc) > 0;
	for (int i = 0; i < argc; i++) {
		ok &= printf("argv[%d]=%s\n", i, argv[i]) > 0;
	}
	ok &= !fflush(stdout);

	ok &= std_report(STDIN_FILENO, "STDIN_FILENO", "I\n");
	ok &= std_report(STDOUT_FILENO, "STDOUT_FILENO", "O\n");
	ok &= std_report(STDERR_FILENO, "STDER_FILENO", "E\n");

	for (int fd = 0; fd < 4096; fd++) {
		if (fcntl(fd, F_GETFL) == -1)
			continue;

		switch (fd) {
		case STDIN_FILENO:
		case STDOUT_FILENO:
		case STDERR_FILENO:
			break;

		default:
			ok &= printf("unknown open fd %d\n", fd) > 0;
			ok &= !fflush(stdout);
			break;
		}
	}
	return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
