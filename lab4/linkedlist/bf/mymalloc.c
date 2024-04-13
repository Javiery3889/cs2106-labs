#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length

// Do not change this. Used by the test harness.
// You may however use this function in your code if necessary.
long get_index(void *ptr) {
    if(ptr == NULL)
        return -1;
    else
        return (long) ((char *) ptr - &_heap[0]);
}

void print_memlist() {
    TNode *curr = _memlist;
    while (curr) {
        TData *data = curr->pdata;
        if (data->is_free) {
            printf("Status: FREE Start index: %d Length: %zu\n", curr->key, data->size);
        } else {
            printf("Status: ALLOCATED Start index: %d Length: %zu\n", curr->key, data->size);
        }
        curr = curr->next;
    }
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    if (_memlist == NULL) {
        TData *data = malloc(sizeof(TData));
        data->size = MEMSIZE;
        data->is_free = 1;
        _memlist = make_node(0, data);
    }

    TNode *curr = _memlist;
    TData *data = malloc(sizeof(TData));
    data->size = SIZE_MAX;
    data->is_free = 1;
    TNode *min_node = NULL;


    while (curr) {
        if (curr->pdata->is_free && size <= curr->pdata->size) {
            if (min_node == NULL) {
                min_node = make_node(0, data);
            }
            if (min_node->pdata->size >= curr->pdata->size) {
                min_node = curr;
            }
        }
        curr = curr->next;
    }
    if (min_node == NULL) {
        return NULL;
    }

    data->size = size;
    data->is_free = 0;
    TNode *new_node = make_node(min_node->key, data);
    insert_node(&_memlist, new_node, ASCENDING);

    min_node->key += size;
    min_node->pdata->size -= size;

    if (min_node->pdata->size == 0) {
        delete_node(&_memlist, min_node);
    }
    return &_heap[new_node->key];
}

// Frees memory pointer to by ptr.
void myfree(void *ptr) {
    long idx = get_index(ptr);
    TNode *node = find_node(_memlist, idx);
    if (idx != -1 && node != NULL) {
        node->pdata->is_free = 1;
        TNode *next = node->next;
        TNode *prev = node->prev;
        while (next && next->pdata->is_free) {
            node->pdata->size += next->pdata->size;
            next = next->next;
            merge_node(_memlist, node, 1);
        }
        while (prev && prev->pdata->is_free) {
            prev->pdata->size += node->pdata->size;
            prev = prev->prev;
            merge_node(_memlist, node, 0);
        }
    }
}

