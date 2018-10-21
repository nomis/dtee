#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	struct timespec ts = { 0, 10 * 1000 * 1000 }; // 10ms
	struct stat canary_stat = { .st_size = 0 };

	if (argc < 3) {
		return EX_USAGE;
	}

	const char *canary_file = argv[1];

	// Signal to close the other side of the fifo
	if (open(canary_file, O_CREAT | O_EXCL | O_CLOEXEC | O_RDWR, S_IRUSR | S_IWUSR) < 0) {
		perror("open");
		return EXIT_FAILURE;
	}

	// Wait for success
	alarm(10);
	errno = 0;
	while (stat(canary_file, &canary_stat) == 0 || errno != ENOENT) {
		nanosleep(&ts, NULL);
		errno = 0;
	}
	alarm(0);

	execvp(argv[2], &argv[2]);
	abort();
	return EXIT_SUCCESS;
}
