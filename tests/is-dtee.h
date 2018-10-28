#ifndef DTEE_IS_DTEE_H_
#define DTEE_IS_DTEE_H_

#include <stdbool.h>

bool dtee_test_is_dtee(void);
bool dtee_test_is_ppid_dtee(void);
bool dtee_test_is_dtee_test(void);

bool __dtee_test_is_dtee(const char *exe_name);
bool __dtee_test_is_dtee_test(const char *exe_name);

#endif
