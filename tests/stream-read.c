#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
		char exp_buf[PIPE_BUF];
		char act_buf[PIPE_BUF];
		ssize_t len;

		for (int i = 0; i < PIPE_BUF; i++) {
			exp_buf[i] = c;

			if (c == ASCII_END) {
				c = ASCII_START;
			} else {
				c++;
			}
		}

		len = read(STDIN_FILENO, act_buf, sizeof(act_buf));
		if (len != (ssize_t)sizeof(act_buf)) {
			fprintf(stderr, "short read of %zd at count=%d\n", len, count + 1);
			fflush(stderr);
			return EXIT_FAILURE;
		}

		if (memcmp(act_buf, exp_buf, PIPE_BUF)) {
			fprintf(stderr, "data failure at count=%d\n", count + 1);
			fflush(stderr);
			return EXIT_FAILURE;
		}
	}

	char buf[PIPE_BUF];
	ssize_t len = read(STDIN_FILENO, buf, sizeof(buf));
	if (len != 0) {
		fprintf(stderr, "additional data of at least %zd at end\n", len);
		fflush(stderr);
		return EXIT_FAILURE;
	}

	printf("OK\n");
	fflush(stdout);
	return EXIT_SUCCESS;
}

