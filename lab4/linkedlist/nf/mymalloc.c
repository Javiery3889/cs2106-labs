#include <stdio.h>
#include <stdlib.h>
#include "mymalloc.h"
#include "llist.h"

char _heap[MEMSIZE] = {0};
TNode *_memlist = NULL; // To maintain information about length
// create a TNode variable that points to previously allocated node
TNode *prev_allocated = NULL;

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
            printf("Status: FREE Start index: %d Length: %d\n", curr->key, data->val);
        } else {
            printf("Status: ALLOCATED Start index: %d Length: %d\n", curr->key, data->val);
        }
        curr = curr->next;
    }
}

// Allocates size bytes of memory and returns a pointer
// to the first byte.
void *mymalloc(size_t size) {
    if (_memlist == NULL) {
        TData *data = malloc(sizeof(TData));
        data->val = MEMSIZE;
        data->is_free = 1;
        _memlist = make_node(0, data);
    }

    TNode *curr;

    // if prev_allocated not null, traversal continues form prev allocated node
    if (prev_allocated) {
        curr = prev_allocated;
    } else {
        curr = _memlist;
    }

    while (curr) {
        if (curr->pdata->is_free && size <= curr->pdata->val) {
            TData *data = malloc(sizeof(TData));
            data->val = size;
            data->is_free = 0;
            TNode *new_node = make_node(curr->key, data);
            // update prev_allocated node each time
            prev_allocated = new_node;
            insert_node(&_memlist, new_node, ASCENDING);

            curr->key += size;
            curr->pdata->val -= size;

            if (curr->pdata->val == 0) {
                delete_node(&_memlist, curr);
            }
            return &_heap[new_node->key];
        }
        curr = curr->next;
    }
    return NULL;
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
            node->pdata->val += next->pdata->val;
            next = next->next;
            merge_node(_memlist, node, 1);
        }
        while (prev && prev->pdata->is_free) {
            prev->pdata->val += node->pdata->val;
            prev = prev->prev;
            merge_node(_memlist, node, 0);
        }
    }
}

