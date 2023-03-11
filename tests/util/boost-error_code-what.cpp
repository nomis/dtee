#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>
#include <unistd.h>

#include <boost/version.hpp>
#include <boost/system/error_code.hpp>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		return EX_USAGE;
	}

	FILE *file = fopen(argv[1], "w");

	if (!file) {
		perror(argv[1]);
		return EX_CANTCREAT;
	}

#if BOOST_VERSION >= 107800
# define STRERROR_FOR(__errnum) \
	fprintf(file, "%s=\"%s\"\n", \
		"BOOST_ERROR_CODE_" #__errnum, \
		boost::system::error_code(static_cast<boost::system::errc::errc_t>(__errnum), boost::system::system_category()).what().c_str());

#else
# define STRERROR_FOR(__errnum) \
	fprintf(file, "%s=\"%s\"\n", \
		"BOOST_ERROR_CODE_" #__errnum, \
		boost::system::error_code(static_cast<boost::system::errc::errc_t>(__errnum), boost::system::system_category()).message().c_str());

#endif

#include "for-each-strerror.h"

#undef STRERROR_FOR

	fclose(file);
	return EXIT_SUCCESS;
}
