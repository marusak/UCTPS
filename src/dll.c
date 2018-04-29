/*
 * Double linked list
 *
 * Author: Matej Marusak
 */

#include "dll.h"
#include "error.h"

// Create new double linked list
dll_t* dll() {
    dll_t *new_dll = (dll_t*)safe_malloc(sizeof(dll_t));
    new_dll->first = NULL;
    new_dll->last = NULL;
    return new_dll;
}

// Insert after item
item* insert_after(dll_t* dll, item* prev, int value, int count){
    item* new_item = (item*)safe_malloc(sizeof(item));
    new_item->value = value;
    new_item->count = count;

    // Inserting after last
    if (prev == dll->last) {
        new_item->prev = dll->last;
        new_item->next = NULL;
        dll->last = new_item;
        prev->next = new_item;
        return new_item;
    }

    new_item->next = prev->next;
    prev->next->prev = new_item;
    prev->next = new_item;
    new_item->prev = prev;

    return new_item;
}

// Insert before item
item* insert_before(dll_t* dll, item* next, int value, int count){
    item* new_item = (item*)safe_malloc(sizeof(item));
    new_item->value = value;
    new_item->count = count;

    // Inserting before first item
    if (next == dll->first){
        new_item->next = dll->first;
        dll->first->prev = new_item;
        new_item->prev = NULL;
        dll->first = new_item;
        return new_item;
    }

    new_item->prev = next->prev;
    next->prev->next = new_item;
    new_item->next = next;
    next->prev = new_item;

    return new_item;
}

// Insert to the last place
item* insert_last(dll_t* dll, int value, int count){
    // Empty dll
    if (dll->first == dll->last && dll->first == NULL) {
        item* new_item = (item*)safe_malloc(sizeof(item));
        new_item->value = value;
        new_item->count = count;

        new_item->prev = NULL;
        new_item->next = NULL;
        dll->last = new_item;
        dll->first = new_item;
        return new_item;
    }

    return insert_after(dll, dll->last, value, count);
}

// Insert to the first place
item* insert_first(dll_t* dll, int value, int count){
    // Empty dll
    if (dll->first == dll->last && dll->first == NULL) {
        item* new_item = (item*)safe_malloc(sizeof(item));
        new_item->value = value;
        new_item->count = count;

        new_item->prev = NULL;
        new_item->next = NULL;
        dll->last = new_item;
        dll->first = new_item;
        return new_item;
    }

    return insert_before(dll, dll->first, value, count);
}

void drop_item(dll_t* dll, int value){
    item* cur = dll->first;
    while (cur != NULL) {
        if (cur->value == value){
            remove_item(dll, cur);
            return;
        }
        cur = cur->next;
    }

    error("Internal error", INTERNAL_ERROR);
}

item* get_nth(dll_t* dll, int n){
    int i = 0;
    item* cur = dll->first;
    while (cur != NULL) {
        if (i == n)
            return cur;
        else {
            i++;
            cur = cur->next;
        }
    }
    error("Internal error", INTERNAL_ERROR);
    return NULL;
}

int get_count(dll_t* dll, int value){
    item* cur = dll->first;
    while (cur != NULL) {
        if (cur->value == value)
            return cur->count;
        else
            cur = cur->next;
    }
    error("Internal error", INTERNAL_ERROR);
    return 0;
}

void set_count_to_value(dll_t* dll, int value, int count){
    item* cur = dll->first;
    while (cur != NULL) {
        if (cur->value == value){
            cur->count = count;
            break;
        }
        cur = cur->next;
    }
}

int dll_len_with_count(dll_t* dll, int ref_count){
    int count = 0;
    item* cur = dll->first;
    while (cur != NULL) {
        if (cur->count == ref_count)
            count++;
        cur = cur->next;
    }
    return count;
}

void set_count_to_all(dll_t* dll, int value){
    item* cur = dll->first;
    while (cur != NULL) {
        cur->count = value;
        cur = cur->next;
    }
}


void insert_last_if_not_in(dll_t* dll, int value, int count){
    item* cur = dll->first;
    while (cur != NULL) {
        if (cur->value == value){
            return;
        }
        cur = cur->next;
    }
    insert_last(dll, value, count);
}

// Print list
void print_dll(dll_t* dll){
    item* current = dll->first;
    while (current != NULL){
        printf("Next item:\n    value: %d\n    count: %d\n\n",current->value, current->count);
        current = current->next;
    }
}

//Remove one item
void remove_item(dll_t* dll, item* item){
    // Item to be deleted is the first
    if (item == dll->first){
        dll->first = item->next;
        // There is more than one element in the list
        if (item->next != NULL)
            dll->first->prev = NULL;
        else
            dll->last = NULL;
        free(item);
        return;
    }

    if (item == dll->last){
        dll->last = item->prev;
        // There is more than one element in the list
        if (item->prev != NULL)
            dll->last->next = NULL;
        free(item);
        return;
    }

    item->prev->next = item->next;
    item->next->prev = item->prev;
    free(item);
}

void clean_dll(dll_t* dll){
    while(dll->first != NULL)
        remove_item(dll, dll->first);
}

// Remove the whole dll
void teardown(dll_t* dll){
    while (dll->first != NULL)
        remove_item(dll, dll->first);
    free(dll);
}
