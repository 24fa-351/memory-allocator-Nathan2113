#include <stdlib.h>

#ifndef MALLOC_H
#define MALLOC_H

// #define SMALL_MEMORY_POOL_SIZE 1024 * 1024 // 1MB
// #define MEDIUM_MEMORY_POOL_SIZE 1024 * 1024 * 10 // 10MB
// #define LARGE_MEMORY_POOL_SIZE 1024 * 1024 * 100 // 100MB
// #define SMALL_MEMORY_POOL_SIZE 1024 // 1KB
// #define MEDIUM_MEMORY_POOL_SIZE 4096 // 4KB
// #define LARGE_MEMORY_POOL_SIZE 1024 * 1024 // 1MB
#define SMALL_MEMORY_POOL_SIZE 128 // 128 bytes
#define MEDIUM_MEMORY_POOL_SIZE 512 // 512 bytes
#define LARGE_MEMORY_POOL_SIZE 1024 // 1KB
#define MAX_BLOCKS 1000

typedef struct chunk_on_heap {
    int size;
    char* pointer_to_start;
}chunk_on_heap;


// void swap(chunk_on_heap** chunk_a, chunk_on_heap** chunk_b);
// void heapify(int index);
// void insert(chunk_on_heap* chunk); 
// chunk_on_heap* heap_pop();
// // Need to be able to call these just "malloc" and "free" and it works for both memtest_sys and memtest_dev
// char* xmalloc(size_t size);
// void xfree(void *ptr);
// char* xrealloc(char *ptr, size_t size);
// void* get_me_blocks(ssize_t size_of_block);
// void initialize_memory_pool();

void* xmalloc(size_t size);
void xfree(void *ptr);
void* xrealloc(void *ptr, size_t size);
void* get_me_blocks(ssize_t size_of_block);
void initialize_memory_pool();

#endif