#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
	FILE *names = fopen("strsignal.txt", "w");

	if (!names) {
		return EX_CANTCREAT;
	}

	FILE *numbers = fopen("signal.txt", "w");

	if (!numbers) {
		return EX_CANTCREAT;
	}

#define STRSIGNAL_FOR(sig) \
	do { \
		fprintf(names, "%s\n%s\n", strsignal(sig), #sig); \
		fprintf(numbers, "%s=%d\n", #sig, sig); \
	} while (0)

	STRSIGNAL_FOR(SIGCHLD);
	STRSIGNAL_FOR(SIGHUP);
	STRSIGNAL_FOR(SIGINT);
	STRSIGNAL_FOR(SIGKILL);
	STRSIGNAL_FOR(SIGPIPE);
	STRSIGNAL_FOR(SIGTERM);
	STRSIGNAL_FOR(SIGQUIT);

#undef STRSIGNAL_FOR

	fclose(names);
	fclose(numbers);
	return EXIT_SUCCESS;
}
