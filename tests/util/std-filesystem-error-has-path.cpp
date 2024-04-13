#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#if __has_include(<filesystem>)
# include <filesystem>
#else
# include <experimental/filesystem>
#endif

#if __has_include(<filesystem>)
using ::std::filesystem::temp_directory_path;
using ::std::filesystem::filesystem_error;
#else
using ::std::experimental::filesystem::temp_directory_path;
using ::std::experimental::filesystem::filesystem_error;
#endif

int main(int argc, char *argv[]) {
	if (argc != 2) {
		return EX_USAGE;
	}

	if (setenv("TMPDIR", "./dtee@test/tmpdir-does-not-exist", 1)) {
		perror("setenv");
		return EX_OSERR;
	}

	FILE *file = fopen(argv[1], "w");

	if (!file) {
		perror(argv[1]);
		return EX_CANTCREAT;
	}

	bool ok = false;

	try {
		temp_directory_path().string();
	} catch (filesystem_error &e) {
		fprintf(file, "STD_FILESYSTEM_ERROR_HAS_PATH=%d\n", !e.path1().empty());
		ok = true;
	}

	fclose(file);
	return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
