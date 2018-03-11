#include <stdlib.h>
#include "error.h"

void error(char* message, int err_code){
    printf("ERROR: %s\n", message);
    exit(err_code);
}
