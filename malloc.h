#include <stdlib.h>

#ifndef MALLOC_H
#define MALLOC_H

#define MAX_BLOCKS 1000

typedef struct chunk_on_heap {
    int size;
    char* pointer_to_start;
}chunk_on_heap;

// Need to be able to call these just "malloc" and "free" and it works for both memtest_sys and memtest_dev
char* xmalloc(size_t size);
void xfree(void *ptr);
char* xrealloc(char *ptr, size_t size);
void* get_me_blocks(ssize_t size_of_block);

#endif