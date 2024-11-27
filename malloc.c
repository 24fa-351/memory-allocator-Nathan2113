#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "malloc.h"


// char* xmalloc(size_t size) {
//     char *ptr = malloc(size);
//     if (ptr == NULL) {
//         fprintf(stderr, __FILE__ ":%d malloc failed\n", __LINE__);
//         exit(1);
//     }
//     fprintf(stderr, __FILE__ ":%d malloc(%lu) = %p\n", __LINE__, size, ptr);
//     return ptr;
// }


// Global free list (using an array to hold the free chunks)
// static chunk_on_heap* free_list[MAX_BLOCKS];
// static int free_list_size = 0;  // Size of the free list (number of chunks in the free list)


// Function to swap two chunks
void swap(chunk_on_heap** chunk_a, chunk_on_heap** chunk_b) {
    chunk_on_heap* temp = *chunk_a;
    *chunk_a = *chunk_b;
    *chunk_b = temp;
}


// Min-heapify function (to maintain the heap property)
void heapify(int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap_size && heap[left]->size < heap[smallest]->size) {
        smallest = left;
    }

    if (right < heap_size && heap[right]->size < heap[smallest]->size) {
        smallest = right;
    }

    if (smallest != index) {
        swap(&heap[index], &heap[smallest]);
        heapify(smallest);
    }
}


// Function to insert a chunk into the heap
void heap_insert(chunk_on_heap* chunk) {
    if (heap_size < MAX_BLOCKS) {
        heap[heap_size] = chunk;
        int index = heap_size;
        heap_size++;

        // Bubble up to maintain the heap property (min-heap)
        while (index > 0 && heap[(index - 1) / 2]->size > heap[index]->size) {
            chunk_on_heap* temp = heap[(index - 1) / 2];
            heap[(index - 1) / 2] = heap[index];
            heap[index] = temp;
            index = (index - 1) / 2;
        }
    } else {
        fprintf(stderr, __FILE__ ":%d Heap is full!\n", __LINE__);
    }
}


chunk_on_heap* heap_pop() {
    if (heap_size == 0) {
        return NULL;
    }

    chunk_on_heap* root = heap[0];
    heap[0] = heap[heap_size - 1];
    heap_size--;

    // Bubble down to restore heap property
    int index = 0;
    while (index * 2 + 1 < heap_size) {
        int left = index * 2 + 1;
        int right = index * 2 + 2;
        int smallest = index;

        if (left < heap_size && heap[left]->size < heap[smallest]->size) {
            smallest = left;
        }
        if (right < heap_size && heap[right]->size < heap[smallest]->size) {
            smallest = right;
        }
        if (smallest == index) {
            break;
        }

        chunk_on_heap* temp = heap[index];
        heap[index] = heap[smallest];
        heap[smallest] = temp;
        index = smallest;
    }

    return root;
}

char* xmalloc(size_t size) {
    printf("Performing custom malloc(%lu)\n", size);
    // Adjust size to include space for the chunk_on_heap struct
    size_t total_size = size + sizeof(chunk_on_heap);
    
    // Try to find a suitable chunk from the heap (smallest chunk that fits)
    chunk_on_heap* current_chunk = heap_pop();

    if (current_chunk && current_chunk->size >= total_size) {
        // Found a suitable chunk
        if (current_chunk->size > total_size + sizeof(chunk_on_heap)) {
            // Split the chunk if it's too large
            chunk_on_heap* new_chunk = (chunk_on_heap*)(((char*)current_chunk) + total_size);
            new_chunk->size = current_chunk->size - total_size;
            new_chunk->pointer_to_start = (char*)new_chunk + sizeof(chunk_on_heap);
            heap_insert(new_chunk);  // Insert the remaining chunk back into the heap
        }

        // Return the usable memory (after the chunk_on_heap header) as char*
        memset(current_chunk->pointer_to_start, 0, size); // Zero out allocated memory
        return (char*)current_chunk->pointer_to_start;
    }
    
    // No suitable chunk found, request more memory using sbrk
    current_chunk = (chunk_on_heap*)get_me_blocks(total_size);
    if (current_chunk == (void*)-1) {
        return NULL; // sbrk failed
    }
    
    current_chunk->size = total_size;
    current_chunk->pointer_to_start = (char*)current_chunk + sizeof(chunk_on_heap);

    printf("Completed custom malloc(%lu)\n", size);

    // Return the usable memory (after the chunk_on_heap header) as char*
    return (char*)current_chunk->pointer_to_start;
}


// void xfree(void *ptr) {
//     fprintf(stderr, __FILE__ ": free(%p)\n", ptr);
//     free(ptr);
// }

void xfree(void* ptr) {
    printf("Performing custom free(%p)\n", ptr);

    // Get the chunk_on_heap struct from the pointer (ptr points after the chunk_on_heap header)
    chunk_on_heap* chunk = (chunk_on_heap*)(((char*)ptr) - sizeof(chunk_on_heap));

    // // Add the chunk to the free list (if there is space)
    // if (free_list_size < MAX_BLOCKS) {
    //     free_list[free_list_size++] = chunk;
    // } else {
    //     printf("Error: Free list is full!\n");
    // }

    heap_insert(chunk);

    printf("Completed custom free(%p)\n", ptr);
}

// void* xrealloc(void *ptr, size_t size) {
//     printf("Performing custom realloc(%p, %lu)\n", ptr, size);
//     void *new_ptr = realloc(ptr, size);

//     if (new_ptr == NULL && size != 0) {
//         fprintf(stderr, __FILE__ ":%d realloc failed\n", __LINE__);
//         exit(1);
//     }

//     fprintf(stderr, __FILE__ ":%d realloc(%p, %lu) = %p\n", __LINE__, ptr, size, new_ptr);

//     return new_ptr;

//     printf("Completed custom realloc(%p, %lu)\n", ptr, size);
// }


char* xrealloc(char* ptr, size_t new_size) {
    if (ptr == NULL) {
        // If the pointer is NULL, realloc behaves like malloc
        return xmalloc(new_size);
    }

    if (new_size == 0) {
        // If the new size is 0, realloc behaves like free
        xfree(ptr);
        return NULL;
    }

    // Get the chunk_on_heap struct from the pointer (ptr points after the chunk_on_heap header)
    chunk_on_heap* chunk = (chunk_on_heap*)(((char*)ptr) - sizeof(chunk_on_heap));
    size_t old_size = chunk->size - sizeof(chunk_on_heap);

    if (new_size <= old_size) {
        // If the new size is smaller or equal to the old size, just return the same pointer
        return ptr;
    }

    // Allocate a new block with the new size
    char* new_ptr = xmalloc(new_size);
    if (new_ptr == NULL) {
        fprintf(stderr, __FILE__ ":%d Malloc failed in realloc!\n", __LINE__);
        return NULL;  // Allocation failed
    }

    // Copy the old data to the new block
    memcpy(new_ptr, ptr, old_size);

    // Free the old block
    xfree(ptr);

    return new_ptr;
}

void* get_me_blocks(ssize_t size_of_block) {
    void* ptr = sbrk(0);
    sbrk(size_of_block);

    return ptr;
}