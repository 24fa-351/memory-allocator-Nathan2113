#include <stdlib.h>

#ifndef MALLOC_H
#define MALLOC_H

#define MAX_BLOCKS 1000

typedef struct chunk_on_heap {
    int size;
    char* pointer_to_start;
}chunk_on_heap;

// Min-heap structure (dynamic array)
static chunk_on_heap* heap[MAX_BLOCKS];
static int heap_size = 0; // Number of chunks on the heap

void swap(chunk_on_heap** chunk_a, chunk_on_heap** chunk_b);
void heapify(int index);
void insert(chunk_on_heap* chunk); 
chunk_on_heap* heap_pop();
// Need to be able to call these just "malloc" and "free" and it works for both memtest_sys and memtest_dev
char* xmalloc(size_t size);
void xfree(void *ptr);
char* xrealloc(char *ptr, size_t size);
void* get_me_blocks(ssize_t size_of_block);

#endif