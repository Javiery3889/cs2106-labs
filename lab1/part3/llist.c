// Implement a double linked-list

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "llist.h"

// Initialize the linked list by setting head to NULL
// PRE: head is the pointer variable that points to the
//      first node of the linked list
// POST: head is set to NULL

void init_llist(TLinkedList **head) {
    // Set head to NULL
    // Rui Bin:
    if (*head != NULL) {
        free(*head);
        *head = NULL;
    }
}

// Create a new node
// PRE: filename = name of file to be inserted
//      filesize = size of file in blocks
//      startblock = File's starting block
// RETURNS: A new node containing the file information is created.

TLinkedList *create_node(char *filename, int filesize, int startblock) {
    // Rui Bin:
    TLinkedList *tll = (TLinkedList *) malloc(sizeof(TLinkedList));
    // no need to malloc filename i think   
    strncpy(tll->filename, filename, MAX_FNAME_LEN);
    tll->filesize = filesize;
    tll->startblock = startblock;
    tll->next = NULL;
    tll->prev = NULL;

    return tll;
}

// Insert node into the end of the linkedlist indicated by head
// PRE: head = Pointer variable pointing to the start of the linked list
//      node = Node created using create_node
// POST: node is inserted into the linked list.

void insert_llist(TLinkedList **head, TLinkedList *node) {
    // rb:
    if (*head == NULL) {
        *head = node;
        return;
    }
    TLinkedList *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = node;
    node->prev = current;
}

// Delete node from the linkedlist
// PRE: head = Pointer variable pointing to the start of the linked list
//      node = An existing node in the linked list to be deleted.
// POST: node is deleted from the linked list

void delete_llist(TLinkedList **head, TLinkedList *node) {
    // rb:
    TLinkedList *current = *head;
    if (current == NULL) {
        return;
    }
    // if (current == node) {
    //     TLinkedList *next = current->next;
    //     next->prev = NULL;
    //     free(current);
    // }
    while (current != node) {
        current = current->next;
    }
    TLinkedList *prev = current->prev;
    TLinkedList *next = current->next;
    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    if (current == *head) {
        if (current->next != NULL) {
            *head = current->next;
        } else {
            *head = NULL;
        }
    }
    free(current);
    // head = &current;
}


// Find node in the linkedlist
// PRE: head = Variable that points to the first node of the linked list
//      fname = Name of file to look for
// RETURNS: The node that contains fname, or NULL if not found.
TLinkedList *find_llist(TLinkedList *head, char *fname) {
    TLinkedList *current = head;
    while (current != NULL) {
        if (strcmp(current->filename, fname) == 0) return current;
        current = current->next;
    }
}

// Traverse the entire linked list calling a function
// PRE: head = Variable pointing to the first node of the linked list
//      fn = Pointer to function to be called for each node
// POST: fn is called with every node of the linked list.

void traverse(TLinkedList **head, void (*fn)(TLinkedList *)) {
    TLinkedList *current = *head;
    while (current != NULL) {
        fn(current);
        current = current->next;
    }
}

