#include "is-dtee.h"

#include <libgen.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static int argc_copy = 0;
static char **argv_copy = NULL;

__attribute__((constructor)) void dtee_test_is_dtee_main_copy(int argc, char **argv) {
	argc_copy = argc;
	argv_copy = argv;
}

bool dtee_test_is_dtee(void) {
	bool is_dtee = false;

	if (argc_copy > 0) {
		char *argv0_copy = strdup(argv_copy[0]);
		if (argv0_copy != NULL) {
			const char *base_program_name = basename(argv0_copy);

			is_dtee = !strcmp(base_program_name, "dtee");
			free(argv0_copy);
		}
	}

	return is_dtee;
}
