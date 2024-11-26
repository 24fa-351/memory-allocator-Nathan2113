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
static chunk_on_heap* free_list[MAX_BLOCKS];
static int free_list_size = 0;  // Size of the free list (number of chunks in the free list)

char* xmalloc(size_t size) {
    printf("Performing custom malloc(%lu)\n", size);
    // Adjust size to include space for the chunk_on_heap struct
    size_t total_size = size + sizeof(chunk_on_heap);
    
    // Search for a suitable chunk in the free list
    for (int i = 0; i < free_list_size; i++) {
        chunk_on_heap* current_chunk = free_list[i];
        if (current_chunk->size >= total_size) {
            // Found a chunk large enough
            if (current_chunk->size > total_size + sizeof(chunk_on_heap)) {
                // Split the chunk if it is large enough
                chunk_on_heap* new_chunk = (chunk_on_heap*)(((char*)current_chunk) + total_size);
                new_chunk->size = current_chunk->size - total_size;
                new_chunk->pointer_to_start = (char*)new_chunk + sizeof(chunk_on_heap);  // Set the start pointer

                // Move the new chunk to the free list
                free_list[free_list_size++] = new_chunk;
            }
            
            // Remove this chunk from the free list by swapping with the last element
            free_list[i] = free_list[--free_list_size];

            // Return the usable memory (after the chunk_on_heap header) as char*
            return (char*)current_chunk->pointer_to_start;
        }
    }
    
    // No suitable chunk found, request more memory using sbrk
    chunk_on_heap* current_chunk = (chunk_on_heap*)get_me_blocks(total_size);
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

    // Add the chunk to the free list (if there is space)
    if (free_list_size < MAX_BLOCKS) {
        free_list[free_list_size++] = chunk;
    } else {
        printf("Error: Free list is full!\n");
    }

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
        printf("Error: xmalloc failed in xrealloc\n");
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