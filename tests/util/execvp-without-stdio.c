#include <sys/resource.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc < 3) {
		printf("%s <[I][O][E]> COMMAND [ARG]...\n", argv[0]);
		return EX_USAGE;
	}

	if (strchr(argv[1], 'I') != NULL) {
		close(STDIN_FILENO);
	}
	if (strchr(argv[1], 'O') != NULL) {
		close(STDOUT_FILENO);
	}
	if (strchr(argv[1], 'E') != NULL) {
		close(STDERR_FILENO);
	}

	execvp(argv[2], &argv[2]);
	perror("execvp");
	return EXIT_FAILURE;
}
