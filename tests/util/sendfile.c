#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc != 3) {
		printf("Usage: %s <input file> <output stream>\n", argv[0]);
		return EX_USAGE;
	}

	int out_fd;
	if (!strcmp(argv[2], "STDOUT_FILENO")) {
		out_fd = STDOUT_FILENO;
	} else if (!strcmp(argv[2], "STDERR_FILENO")) {
		out_fd = STDERR_FILENO;
	} else {
		return EX_USAGE;
	}

	int in_fd = open(argv[1], O_RDONLY);
	if (in_fd < 0) {
		perror("open");
		return EX_NOINPUT;
	}

	struct stat in_stat;
	if (fstat(in_fd, &in_stat) < 0) {
		perror("stat");
		close(in_fd);
		return EX_NOINPUT;
	}

	if (sendfile(out_fd, in_fd, NULL, in_stat.st_size) != in_stat.st_size) {
		perror("sendfile");
		close(in_fd);
		return EX_IOERR;
	}

	close(in_fd);
	return EXIT_SUCCESS;
}
