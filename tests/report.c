#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static bool std_report(int fd, const char *name, const char *message) {
	char buf[1] = { 0 };

	errno = 0;
	if (fcntl(fd, F_GETFL) != -1) {
		ssize_t rlen = read(fd, buf, 1);
		ssize_t wlen = write(fd, message, strlen(message));
		printf("fd %s open read=%zd (%02x) write=%zd\n", name, rlen, buf[0], wlen);
		fflush(stdout);
		return true;
	} else {
		printf("fd %s closed errno=%d\n", name, errno);
		fflush(stdout);
		return false;
	}
}

int main(int argc, char *argv[]) {
	bool ok = true;

	printf("argc=%d\n", argc);
	for (int i = 0; i < argc; i++) {
		printf("argv[%d]=%s\n", i, argv[i]);
	}
	fflush(stdout);

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
			printf("unknown open fd %d\n", fd);
			fflush(stdout);
			ok = false;
			break;
		}
	}
	return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
