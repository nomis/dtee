#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int mkostemp(char *template, int flags) {
	// Not a safe implementation of mkostemp but the filename needs to be predictable
	unlink(template);
	flags &= ~(O_RDONLY | O_RDWR);
	flags |= O_WRONLY | O_CREAT | O_EXCL;
	return open(template, flags, S_IWUSR);
}

int mkostemp64(char *template, int flags) {
	// Not a safe implementation of mkostemp but the filename needs to be predictable
	unlink(template);
	flags &= ~(O_RDONLY | O_RDWR);
	flags |= O_WRONLY | O_CREAT | O_EXCL;
	return open(template, flags, S_IWUSR);
}
