#ifndef DTEE_ALLOW_N_TIMES_H_
#define DTEE_ALLOW_N_TIMES_H_

#include <stdbool.h>

bool dtee_test_allow_n_times(const char *env_name, bool *first, unsigned long *allowed);

#endif
