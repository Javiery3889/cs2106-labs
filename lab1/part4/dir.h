#include "llist.h"

// Add prototypes and other declarations below
void init_hashtable(TLinkedList *hashtable[], int len);

void add_file(char *filename, int filesize, int startblock, int (*hashfun)(char *, int),
   TLinkedList *hashtable[], int len);

void listdir(TLinkedList *hashtable[], int len);

TLinkedList *find_file(char *filename, int (*hashfun)(char *, int), TLinkedList *hashtable[], int len);

void rename_file(char *old_filename, char *new_filename, int (*hashfun)(char *, int),
    TLinkedList *hashtable[], int len);

void delete_file(char *filename, int (*hashfun)(char *, int), TLinkedList *hashtable[], int len);

void free_all_lists(TLinkedList *hashtable[], int len);