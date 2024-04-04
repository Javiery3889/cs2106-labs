#include <stdio.h>
#include "bitmap.h"

// IMPLEMENTED FOR YOU
// Utility function to print out an array of char as bits
// Print the entire bitmap. Arguments: The bitmap itself, and the length of the bitmap 
// Each bit of the bitmap represents 1 byte of memory. 0 = free, 1 = allocated.
// map: The bitmap itself, an array of unsigned char.
//      Each bit of the bitmap represents one byte of memory
// len: The length of the bitmap array in characters
//
// Returns: Nothing
void print_map(unsigned char *map, int len) {
    int i, j;

    for(i=0; i<len; i++) {

        unsigned char mask = 0b10000000;
        for(j=0; j<8; j++) {
            if(map[i] & mask)
                printf("1");
            else
                printf("0");
            mask = mask >> 1;
        }
        printf(" ");
    }
    printf("\n");
}

// Search the bitmap for the required number of zeroes (representing
// free bytes of memory). Returns index of first stretch of 0s
// that meet the criteria. You can use this as an index into
// an array of char that represents the process heap
// bitmap = Bitmap declared as an array of unsigned char
// len = Length of bitmap in characters
// num_zeroes = Length of string of 0's required
// Returns: Index to stretch of 0's of required length, -1 if no such stretch can be found

long search_map(unsigned char *bitmap, int len, long num_zeroes) {
    long num_bits = len * 8;
    if (num_bits < num_zeroes) {
        return -1;
    }

    long count = 0;
    for (long i = 0; i < num_bits; i++) {
        int byte_idx = i / 8;
        int offset = i % 8;
        unsigned char mask = 1 << (7 - offset);
        if (!(bitmap[byte_idx] & mask)) {
            count++;
            if (count == num_zeroes) {
                return i - num_zeroes + 1;
            }
        } else {
            count = 0;
        }
    }
    return -1;
}

// Set map bits to 0 or 1 depending on whether value is non-zero
// map = Bitmap, declared as an array of unsigned char
// start = Starting index to mark as 1 or 0
// length = Number of bits to mark
// value = Value to mark the bits as. value = 0 marks the bits
//          as 0, non-zero marks the bits as 1
// Returns: Nothing

void set_map(unsigned char *map, long start, long length, int value) {
    long bits_from_start = length + start;
    for (long i = start; i < bits_from_start; i++) {
        int byte_idx = i >> 3;
        int offset = i % 8;
        unsigned char mask = 1 << (7 - offset);

        if (value) {
            map[byte_idx] |= mask;
        } else {
            map[byte_idx] &= ~mask;
        }
    }
}

// Marks a stretch of bits as "1", representing allocated memory
// map = Bitmap declared as array of unsigned char
// start = Starting index to mark
// length = Number of bits to mark as "1"
void allocate_map(unsigned char *map, long start, long length) {
    set_map(map, start, length, 1);

}

// Marks a stretch of bits as "0", representing allocated memory
// map = Bitmap declared as array of unsigned char
// start = Starting index to mark
// length = Number of bits to mark as "0"
void free_map(unsigned char *map, long start, long length) {
    set_map(map, start, length, 0);
    
}

