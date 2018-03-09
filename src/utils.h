/*
 * Basic utilites
 *
 * Author: Matej Marusak
 */

#ifndef UCTPS_UTILS_H
#define UCTPS_UTILS_H

#include <stdlib.h>
#include <stdio.h>

// Alloc memory and make sure it was successful
void* safe_malloc(size_t size);

// Read one line from file
void get_line(char* buffer, int max_size, FILE* fl);

// Split line to integers
long get_number(char** line);

// Return 1 if '1' and 0 if '0'
int get_binary(char x);

// Convert list of ints into string of space separated values
char* ints_to_string(int* values, int count);

#endif
