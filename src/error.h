#include <stdio.h>

#define INPUT_FILE_ERROR 1
#define NO_FEASIBLE_SOLUTION 2
#define MALLOC_ERROR 3
#define INTERNAL_ERROR 4

void error(char* message, int err_code);
