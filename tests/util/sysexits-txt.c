#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
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

#define VALUE_OF(status) \
	do { \
		fprintf(file, "%s=%d\n", #status, status); \
	} while (0)

	VALUE_OF(EX_OK);
	VALUE_OF(EX_USAGE);
#ifdef EX_DATAERR
	VALUE_OF(EX_DATAERR);
#endif
#ifdef EX_NOINPUT
	VALUE_OF(EX_NOINPUT);
#endif
#ifdef EX_NOUSER
	VALUE_OF(EX_NOUSER);
#endif
#ifdef EX_NOHOST
	VALUE_OF(EX_NOHOST);
#endif
	VALUE_OF(EX_UNAVAILABLE);
	VALUE_OF(EX_SOFTWARE);
	VALUE_OF(EX_OSERR);
#ifdef EX_OSFILE
	VALUE_OF(EX_OSFILE);
#endif
	VALUE_OF(EX_CANTCREAT);
	VALUE_OF(EX_IOERR);
#ifdef EX_TEMPFAIL
	VALUE_OF(EX_TEMPFAIL);
#endif
#ifdef EX_PROTOCOL
	VALUE_OF(EX_PROTOCOL);
#endif
#ifdef EX_NOPERM
	VALUE_OF(EX_NOPERM);
#endif
#ifdef EX_CONFIG
	VALUE_OF(EX_CONFIG);
#endif

#undef VALUE_OF

	fclose(file);
	return EXIT_SUCCESS;
}
