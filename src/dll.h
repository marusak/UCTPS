/*
 * Double linked list
 *
 * Author: Matej Marusak
 */

#ifndef UCTPS_DLL_H
#define UCTPS_DLL_H

#include "utils.h"

typedef struct item_t {
    struct item_t* prev;
    struct item_t* next;
    int value;
    int count;
} item;

typedef struct dll_t {
    item* first;
    item* last;
} dll_t;

// Create new double linked list
dll_t* dll();

// Insert after item
item* insert_after(dll_t* dll, item* prev, int value, int count);

// Insert before item
item* insert_before(dll_t* dll, item* next, int value, int count);

// Insert to the last place
item* insert_last(dll_t* dll, int value, int count);

// Insert to the first place
item* insert_first(dll_t* dll, int value, int count);

// Print list
void print_dll(dll_t* dll);

//Remove one item
void remove_item(dll_t* dll, item* item);

// Remove the whole dll
void teardown(dll_t* dll);

#endif
