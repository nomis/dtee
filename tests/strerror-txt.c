#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
	FILE *file = fopen("strerror.txt", "w");

	if (!file) {
		return EX_CANTCREAT;
	}

#define STRERROR_FOR(errnum) \
	do { \
		fprintf(file, "%s\n%s\n", strerror(errnum), #errnum); \
	} while (0)

#ifdef EACCES
	STRERROR_FOR(EACCES);
#endif
#ifdef EAFNOSUPPORT
	STRERROR_FOR(EAFNOSUPPORT);
#endif
#ifdef EAGAIN
	STRERROR_FOR(EAGAIN);
#endif
#ifdef EBADF
	STRERROR_FOR(EBADF);
#endif
#ifdef ECHILD
	STRERROR_FOR(ECHILD);
#endif
#ifdef ECONNREFUSED
	STRERROR_FOR(ECONNREFUSED);
#endif
#ifdef EINVAL
	STRERROR_FOR(EINVAL);
#endif
#ifdef EIO
	STRERROR_FOR(EIO);
#endif
#ifdef EMFILE
	STRERROR_FOR(EMFILE);
#endif
#ifdef ENOSPC
	STRERROR_FOR(ENOSPC);
#endif
#ifdef ENOTCONN
	STRERROR_FOR(ENOTCONN);
#endif
#ifdef EPERM
	STRERROR_FOR(EPERM);
#endif
#ifdef EROFS
	STRERROR_FOR(EROFS);
#endif

#undef STRERROR_FOR
	return EXIT_SUCCESS;
}
