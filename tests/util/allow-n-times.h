#pragma once

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool dtee_test_allow_n_times(const char *env_name, bool *first, unsigned long *allowed);

#ifdef __cplusplus
}
#endif
