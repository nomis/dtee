#include <errno.h>
#include <stdlib.h>

int mkostemp(char *template __attribute__((unused)), int flags __attribute__((unused))) {
	errno = EROFS;
	return -1;
}

int mkostemp64(char *template __attribute__((unused)), int flags __attribute__((unused))) {
	errno = EROFS;
	return -1;
}
