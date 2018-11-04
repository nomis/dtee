#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <assert.h>
#include <errno.h>
#include <libgen.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

static int recreate_input_socket(struct sockaddr_un *peer, FILE *output) {
	struct sockaddr_un input = *peer;
	input.sun_path[sizeof(input.sun_path) - 1] = 0;
	input.sun_path[strlen(input.sun_path) - 1] = 'i';

	int input_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
	if (input_fd < 0) {
		fprintf(output, "socket: %s\n", strerror(errno));
		fflush(output);
		exit(EXIT_FAILURE);
	}

	char path[sizeof(input.sun_path)];
	memcpy(path, input.sun_path, sizeof(input.sun_path));

	if (mkdir(dirname(path), S_IRUSR|S_IWUSR|S_IXUSR) < 0) {
		fprintf(output, "mkdir: %s\n", strerror(errno));
		fflush(output);
		exit(EXIT_FAILURE);
	}

	// This will work but it should be a different socket from the one the
	// parent process provided, so writes to stdout/stderr should not work
	if (bind(input_fd, (struct sockaddr*)&input, sizeof(input)) < 0) {
		fprintf(output, "bind: %s\n", strerror(errno));
		fflush(output);
		rmdir(path);
		exit(EXIT_FAILURE);
	}
	unlink(input.sun_path);
	rmdir(path);

	fprintf(output, "Reopened input socket %s\n", input.sun_path);
	fflush(output);
	return input_fd;
}

int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused))) {
	struct timespec ts = { 0, 10 * 1000 * 1000 }; // 10ms
	const char *monitor_stdout = getenv("DTEE_TEST_MONITOR_OUTPUT");
	const char *extra_stdout = getenv("DTEE_TEST_EXTRA_OUTPUT_FD");
	struct stat stdout_stat = { .st_size = 0 };
	pid_t ppid = getppid();
	FILE *output;

	assert(monitor_stdout != NULL);
	assert(extra_stdout != NULL);

	// Get peername before the other side disconnects
	struct sockaddr_un peer = { .sun_family = AF_UNSPEC, .sun_path = { 0 } };
	socklen_t peerlen = sizeof(peer);

	if (getpeername(STDOUT_FILENO, (struct sockaddr*)&peer, &peerlen) < 0) {
		perror("getpeername");
		return EXIT_FAILURE;
	}

	output = fdopen(strtoul(extra_stdout, NULL, 10), "w");
	if (output == NULL) {
		perror("fdopen");
		return EXIT_FAILURE;
	}

	printf("Killing parent process\n");
	fflush(stdout);

	// Wait for the process to read our output before killing it
	alarm(10);
	while (stat(monitor_stdout, &stdout_stat) < 0 || stdout_stat.st_size == 0) {
		nanosleep(&ts, NULL);
	}

	kill(ppid, SIGTERM);

	// Wait for the process to exit after handling the signal
	alarm(10);
	while (kill(ppid, 0) == 0) {
		nanosleep(&ts, NULL);
	}

	fprintf(output, "Parent process exited\n");
	fflush(output);

	int input_fd = recreate_input_socket(&peer, output);

	// Write data after the parent process has exited
	const char *message = "Test message\n";
	ssize_t len = strlen(message);
	if (printf("%s", message) != len || fflush(stdout) == EOF) {
		fprintf(output, "printf/fflush: %s\n", strerror(errno));
		fflush(output);
	}

	// Try to read the data we sent (non-blocking)
	char buf[1024] = { 0 };
	len = recv(input_fd, buf, sizeof(buf) - 1, MSG_DONTWAIT);
	if (len < 0) {
		fprintf(output, "recv: %s\n", strerror(errno));
		fflush(output);
		return EXIT_FAILURE;
	}

	fprintf(output, "Received %zd bytes\n", len);
	fprintf(output, "%s", buf);
	fclose(output);
	return EXIT_SUCCESS;
}
