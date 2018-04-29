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

// Get nth item
item* get_nth(dll_t* dll, int n);

// Get count by value
int get_count(dll_t* dll, int value);

// Set count to item with value
void set_count_to_value(dll_t* dll, int value, int count);

// Insert last if with given value not in list
void insert_last_if_not_in(dll_t* dll, int value, int count);

// Count items with count as given
int dll_len_with_count(dll_t* dll, int ref_count);

// Set count to all items
void set_count_to_all(dll_t* dll, int value);


// Print list
void print_dll(dll_t* dll);

// Remove first item where value is as given
void drop_item(dll_t* dll, int value);

//Remove one item
void remove_item(dll_t* dll, item* item);

//Remove all items from list
void clean_dll(dll_t* dll);

// Remove the whole dll
void teardown(dll_t* dll);

#endif
