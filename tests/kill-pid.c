#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	const char *env_rlimit_core = getenv("DTEE_TEST_RLIMIT_CORE");
	int signum;

	if (argc != 2) {
		return EX_USAGE;
	}

	if (env_rlimit_core != NULL) {
		struct rlimit rlim;
		rlim_t value;

		if (!strcmp(env_rlimit_core, "RLIM_INFINITY")) {
			value = RLIM_INFINITY;
		} else {
			value = strtoul(env_rlimit_core, NULL, 10);
#ifdef __linux__
			// If kernel.core_pattern is a pipe and fs.suid_dumpable is non-zero,
			// then core dumps happen even if RLIMIT_CORE is 0 or the executable is
			// not readable. However, they don't happen if RLIMIT_CORE is the special
			// value of 1.

			if (value == 0) {
				value = 1;
			}

			// When kernel.core_pattern is not a pipe, then RLIMIT_CORE must be at
			// least PAGE_SIZE or ELF_EXEC_PAGESIZE (both of which are greater than 1)
			// for a core dump to be produced.
#endif
		}

		rlim.rlim_cur = rlim.rlim_max = value;

		if (setrlimit(RLIMIT_CORE, &rlim) < 0) {
			perror("setrlimit");
			return EXIT_FAILURE;
		}
	}

	signum = strtol(argv[1], NULL, 10);

	printf("Killing current process\n");
	fflush(stdout);

	kill(getpid(), signum);
	return EXIT_SUCCESS;
}
