/*
 * Header for basic utilies
 *
 * Author: Matej Marusak
 */

#include "utils.h"

void* safe_malloc(size_t size) {
    void* new_memory = malloc(size);
    if (! new_memory)
        exit(10);
    return new_memory;
}
