#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <assert.h>
#include <errno.h>
#include <dlfcn.h>
#include <libgen.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "is-dtee.h"
#include "is-fd-unix-socket.h"
#include "dtee-fcn.h"

static bool have_details = false;
static struct sockaddr_un input;
static struct sockaddr_un output;

// When dtee connects to its own input socket, copy the socket details and make an extra socket
TEST_FCN_REPL(int, connect, (int sockfd, const struct sockaddr *addr, socklen_t addrlen)) {
	int (*next_connect)(int, const struct sockaddr *, socklen_t) = TEST_FCN_NEXT(connect);

	if (dtee_test_is_dtee() && !have_details) {
		struct sockaddr_un sock_addr = { .sun_family = AF_UNSPEC, .sun_path = { 0 } };

		if (dtee_test_is_fd_unix_socket(sockfd, &sock_addr)) {
			sock_addr.sun_path[sizeof(sock_addr.sun_path) - 1] = 0;
			input = sock_addr;
			output = sock_addr;
			input.sun_path[strlen(input.sun_path) - 1] = 'i';
			output.sun_path[strlen(output.sun_path) - 1] = 'o';

			have_details = true;
		}
	}

	return (*next_connect)(sockfd, addr, addrlen);
}

static int dtee_test_recreate_output_socket(FILE *extra) {
	fprintf(extra, "Input socket: %s\n", input.sun_path);
	fprintf(extra, "Output socket: %s\n", output.sun_path);
	fflush(extra);

	int output_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (output_fd < 0) {
		fprintf(extra, "socket: %s\n", strerror(errno));
		fflush(extra);
		return -1;
	}

	char path[sizeof(input.sun_path)];
	memcpy(path, input.sun_path, sizeof(input.sun_path));

	if (mkdir(dirname(path), S_IRUSR|S_IWUSR|S_IXUSR) < 0) {
		fprintf(extra, "mkdir: %s\n", strerror(errno));
		fflush(extra);
		close(output_fd);
		return -1;
	}

	if (bind(output_fd, (struct sockaddr*)&output, sizeof(output)) < 0) {
		fprintf(extra, "bind: %s\n", strerror(errno));
		fflush(extra);
		rmdir(path);
		close(output_fd);
		return -1;
	}
	unlink(output.sun_path);

	// This will not work because it would be a different socket from the
	// original input socket, which no longer exists
	if (connect(output_fd, (struct sockaddr*)&input, sizeof(input)) < 0) {
		fprintf(extra, "connect: %s\n", strerror(errno));
		fflush(extra);
		rmdir(path);
		close(output_fd);
		return -1;
	}
	rmdir(path);

	fprintf(extra, "Recreated output socket\n");
	fflush(extra);
	return output_fd;
}

// The child process has exited, so any remaining data on the input socket will now be read
TEST_FCN_REPL(pid_t, waitpid, (pid_t pid, int *wstatus, int options)) {
	pid_t (*next_waitpid)(pid_t, int *, int) = TEST_FCN_NEXT(waitpid);
	static __thread bool active = false;

	if (!active) {
		active = true;

		if (dtee_test_is_dtee()) {
			const char *extra_stdout = getenv("DTEE_TEST_EXTRA_OUTPUT_FD");
			FILE *extra;

			assert(extra_stdout != NULL);

			extra = fdopen(strtoul(extra_stdout, NULL, 10), "w");
			if (extra == NULL) {
				perror("security-recreate-output-socket fdopen");
				abort();
			}

			if (!have_details) {
				fprintf(extra, "No input socket details available\n");
				fflush(extra);
			}

			int output_fd = dtee_test_recreate_output_socket(extra);
			if (output_fd >= 0) {
				const char *message = "Test message\n";
				ssize_t len = strlen(message);
				if (write(output_fd, message, strlen(message)) != len) {
					fprintf(extra, "write: %s\n", strerror(errno));
					fflush(extra);
				} else {
					fprintf(extra, "Finished writing with recreated output socket\n");
					fflush(extra);
				}

				close(output_fd);
			}

			fclose(extra);
		}

		active = false;
	}

	return (*next_waitpid)(pid, wstatus, options);
}
