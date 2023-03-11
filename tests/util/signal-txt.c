#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		return EX_USAGE;
	}

	FILE *file = fopen(argv[1], "w");

	if (!file) {
		perror(argv[1]);
		return EX_CANTCREAT;
	}

#define SIGNAL_FOR(sig) \
	do { \
		fprintf(file, "%s=%d\n", #sig, sig); \
	} while (0)

	SIGNAL_FOR(SIGCHLD);
	SIGNAL_FOR(SIGHUP);
	SIGNAL_FOR(SIGINT);
	SIGNAL_FOR(SIGKILL);
	SIGNAL_FOR(SIGPIPE);
	SIGNAL_FOR(SIGTERM);
	SIGNAL_FOR(SIGQUIT);

#undef SIGNAL_FOR

	fclose(file);
	return EXIT_SUCCESS;
}
