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

void get_line(char* buffer, int max_size, FILE* fl){
    if (!fgets(buffer, max_size, fl))
        exit(2);
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
    exit(3);
}

char* ints_to_string(int* values, int count){
    char* output = safe_malloc(13 * count);
    char* r_output = output;
    for (int i = 0; i < count; i++){
        int n = sprintf(output, "%d", values[i]);
        if (n >= 1){
            output[n] = ' ';
            output += n + 1;
        }
        else
            exit(11);
    }
    return r_output;
}
