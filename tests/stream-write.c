#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sysexits.h>
#include <unistd.h>

#define ASCII_START 32
#define ASCII_END 126

int main(int argc, char *argv[]) {
	if (argc != 2) {
		return EX_USAGE;
	}

	int count = strtoul(argv[1], NULL, 10);
	char c = ASCII_START;

	count /= PIPE_BUF;

	if (count <= 0) {
		return EX_USAGE;
	}

	while (count-- != 0) {
		char buf[PIPE_BUF];

		for (int i = 0; i < PIPE_BUF; i++) {
			buf[i] = c;

			if (c == ASCII_END) {
				c = ASCII_START;
			} else {
				c++;
			}
		}

		if (write(STDOUT_FILENO, buf, sizeof(buf)) != sizeof(buf)) {
			perror("write");
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

