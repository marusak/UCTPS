/*
 * Header for basic utilies
 *
 * Author: Matej Marusak
 */

#include "utils.h"
#include <string.h>
#include "error.h"

void* safe_malloc(size_t size) {
    void* new_memory = malloc(size);
    if (! new_memory)
        error("Was not able to alloc any more memory", MALLOC_ERROR);
    return new_memory;
}

void get_line(char* buffer, int max_size, FILE* fl){
    if (!fgets(buffer, max_size, fl))
        error("Input file shorter than expected", INPUT_FILE_ERROR);
    buffer[max_size] = '\0';
}

long get_number(char** line){
    return strtol(*line, line, 10);
}

int get_binary(char x){
    if (x == '1')
        return 1;
    if (x == '0')
        return 0;
    error("Input file contains unexpected value", INPUT_FILE_ERROR);
    return 0;
}

char* ints_to_string(int* values, int count){
    char* output = safe_malloc(13 * count);
    memset(output, '\0', 13*count);
    char* r_output = output;
    for (int i = 0; i < count; i++){
        int n = sprintf(output, "%d", values[i]);
        if (n >= 1){
            output[n] = ' ';
            output += n + 1;
        }
        else
            error("Internal error", INTERNAL_ERROR);
    }
    return r_output;
}

int compare(const void* a, const void* b) {
     int int_a = *((int*) a);
     int int_b = *((int*) b);

     if (int_a == int_b)
         return 0;
     else if (int_a < int_b)
         return -1;
     else
         return 1;
}

int count_between(int* tmp, int from, int to, int max_count){
    int count = 0;
    for (int i = 0; i < max_count; i++){
        if (tmp[i] >= from && tmp[i] <= to)
            count++;
        if (tmp[i] > to)
            break;
    }
    return count;
}

bool has_value(int* tmp, int value, int max_count){
    for (int i = 0; i < max_count; i++){
        if (tmp[i] == value)
            return true;
        if (tmp[i] > value)
            return false;
    }
    return false;
}
