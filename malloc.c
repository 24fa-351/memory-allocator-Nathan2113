#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "malloc.h"

static char *memory_pool = NULL;
static size_t memory_pool_size = 0;
static size_t memory_pool_used = 0;

static chunk_on_heap* heap = NULL;
static int heap_size = 0; // Number of chunks on the heap


void initialize_memory_pool() {
    if (memory_pool == NULL) {
        memory_pool = get_me_blocks(LARGE_MEMORY_POOL_SIZE);
        memory_pool_size = LARGE_MEMORY_POOL_SIZE;
        heap = (chunk_on_heap*)memory_pool;
        heap[0].size = memory_pool_size - sizeof(chunk_on_heap);
        heap[0].pointer_to_start = memory_pool + sizeof(chunk_on_heap);
        heap_size = 1;
    }
}

void* get_me_blocks(ssize_t size_of_block) {
    void* ptr = sbrk(0);
    if (sbrk(size_of_block) == (void*) -1) {
        fprintf(stderr, "get_me_blocks: sbrk failed\n");
        exit(1);
    }
    printf("New Memory Reserved: %ld bytes\n", size_of_block);
    return ptr;
}

void heapify_down(int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap_size && heap[left].size < heap[smallest].size) {
        smallest = left;
    }
    if (right < heap_size && heap[right].size < heap[smallest].size) {
        smallest = right;
    }
    if (smallest != index) {
        chunk_on_heap temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        heapify_down(smallest);
    }
}

void heapify_up(int index) {
    int parent = (index - 1) / 2;
    if (index && heap[index].size < heap[parent].size) {
        chunk_on_heap temp = heap[index];
        heap[index] = heap[parent];
        heap[parent] = temp;
        heapify_up(parent);
    }
}

chunk_on_heap extract_min() {
    if (heap_size == 0) {
        fprintf(stderr, "Heap underflow\n");
        exit(1);
    }
    chunk_on_heap root = heap[0];
    heap[0] = heap[--heap_size];
    heapify_down(0);
    return root;
}

void heap_insert(chunk_on_heap chunk) {
    // heap[heap_size++] = chunk;
    // heapify_up(heap_size - 1);
    if (heap_size == memory_pool_size / sizeof(chunk_on_heap)) {
        // Resize heap if it's full
        memory_pool_size *= 2;
        heap = realloc(heap, memory_pool_size);
        if (heap == NULL) {
            fprintf(stderr, "Memory allocation failed during heap resizing\n");
            exit(1);
        }
    }

    heap[heap_size++] = chunk;
    heapify_up(heap_size - 1);
}

void* xmalloc(size_t size) {
    initialize_memory_pool();

    if (size <= 0) {
        return NULL;
    }

    for (int i = 0; i < heap_size; i++) {
        if (heap[i].size >= size) {
            chunk_on_heap chunk = extract_min();
            if (chunk.size > size + sizeof(chunk_on_heap)) {
                chunk_on_heap new_chunk;
                new_chunk.size = chunk.size - size - sizeof(chunk_on_heap);
                new_chunk.pointer_to_start = chunk.pointer_to_start + size + sizeof(chunk_on_heap);
                heap_insert(new_chunk);
            }
            chunk.size = size;
            return chunk.pointer_to_start;
        }
    }

    // Request more memory via get_me_blocks if no suitable chunk is found
    printf("\nNo suitable chunk found\n");
    size_t block_size = size + sizeof(chunk_on_heap);
    void* new_block = get_me_blocks(block_size);
    if (new_block == (void*) -1) {
        fprintf(stderr, "xmalloc: Out of memory\n");
        return NULL;
    }

    chunk_on_heap new_chunk;
    new_chunk.size = size;
    new_chunk.pointer_to_start = (char*)new_block + sizeof(chunk_on_heap);
    heap_insert(new_chunk);

    printf("New Memory Allocated: %ld bytes\n\n\n", size);

    return new_chunk.pointer_to_start;
}

void xfree(void *ptr) {
    if (!ptr) {
        return;
    }

    chunk_on_heap* chunk = (chunk_on_heap*)((char*)ptr - sizeof(chunk_on_heap));
    if(chunk->pointer_to_start == NULL) {
        return;
    }

    chunk->pointer_to_start = (char*)ptr;
    // chunk->pointer_to_start = NULL;
    heap_insert(*chunk);
}

void* xrealloc(void *ptr, size_t size) {
    if (!ptr) {
        return xmalloc(size);
    }

    chunk_on_heap* chunk = (chunk_on_heap*)((char*)ptr - sizeof(chunk_on_heap));
    if (chunk->size >= size) {
        return ptr;
    }

    void *new_ptr = xmalloc(size);
    if (!new_ptr) {
        return NULL;
    }

    memcpy(new_ptr, ptr, chunk->size);
    xfree(ptr);
    return new_ptr;
}