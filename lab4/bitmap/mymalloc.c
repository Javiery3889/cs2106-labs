#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"
#include "bitmap.h"
#define BITMAP_SIZE 8

char _heap[MEMSIZE] = {0};
unsigned char bitmap[BITMAP_SIZE] = {0};
TNode *_memlist = NULL;

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_memlist() {
    // Implement this to call print_map from bitmap.c
    print_map(bitmap, BITMAP_SIZE);
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    long index = search_map(bitmap, BITMAP_SIZE, size);
    if (index == -1) {
        return NULL;
    }

    TData *data = malloc(sizeof(TData));
    data->val = size;
    TNode *node = make_node(index, data);
    insert_node(&_memlist, node, 0);

    allocate_map(bitmap, index, size);
    return &_heap[index];
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    long idx = get_index(ptr);
    TNode *node = find_node(_memlist, idx);
    if (idx != -1 && node != NULL) {
        TData *data = node->pdata;
        int size = data->val;

        free_map(bitmap, idx, size);
        delete_node(&_memlist, node);
    }
}

