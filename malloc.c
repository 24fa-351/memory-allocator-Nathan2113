#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "malloc.h"


char* xmalloc(size_t size) {
    char *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, __FILE__ ":%d malloc failed\n", __LINE__);
        exit(1);
    }
    fprintf(stderr, __FILE__ ":%d malloc(%lu) = %p\n", __LINE__, size, ptr);
    return ptr;
}


// Global free list (using an array to hold the free chunks)
// static chunk_on_heap* free_list[MAX_BLOCKS];
// static int free_list_size = 0;  // Size of the free list (number of chunks in the free list)

// char* xmalloc(size_t size) {
//     // Adjust size to include space for the chunk_on_heap struct
//     size_t total_size = size + sizeof(chunk_on_heap);
    
//     // Search for a suitable chunk in the free list
//     for (int i = 0; i < free_list_size; i++) {
//         chunk_on_heap* curr = free_list[i];
//         if (curr->size >= total_size) {
//             // Found a chunk large enough
//             if (curr->size > total_size + sizeof(chunk_on_heap)) {
//                 // Split the chunk if it is large enough
//                 chunk_on_heap* new_chunk = (chunk_on_heap*)(((char*)curr) + total_size);
//                 new_chunk->size = curr->size - total_size;
//                 new_chunk->pointer_to_start = (char*)new_chunk + sizeof(chunk_on_heap);  // Set the start pointer

//                 // Move the new chunk to the free list
//                 free_list[free_list_size++] = new_chunk;
//             }
            
//             // Remove this chunk from the free list by swapping with the last element
//             free_list[i] = free_list[--free_list_size];

//             // Return the usable memory (after the chunk_on_heap header) as char*
//             return (char*)curr->pointer_to_start;
//         }
//     }
    
//     // No suitable chunk found, request more memory using sbrk
//     chunk_on_heap* curr = (chunk_on_heap*)sbrk(total_size);
//     if (curr == (void*)-1) {
//         return NULL; // sbrk failed
//     }
    
//     curr->size = total_size;
//     curr->pointer_to_start = (char*)curr + sizeof(chunk_on_heap);

//     // Return the usable memory (after the chunk_on_heap header) as char*
//     return (char*)curr->pointer_to_start;
// }


void xfree(void *ptr) {
    fprintf(stderr, __FILE__ ": free(%p)\n", ptr);
    free(ptr);
}

void* xrealloc(void *ptr, size_t size) {
    void *new_ptr = realloc(ptr, size);

    if (new_ptr == NULL && size != 0) {
        fprintf(stderr, __FILE__ ":%d realloc failed\n", __LINE__);
        exit(1);
    }

    fprintf(stderr, __FILE__ ":%d realloc(%p, %lu) = %p\n", __LINE__, ptr, size, new_ptr);

    return new_ptr;
}

void* get_me_blocks(ssize_t size_of_block) {
    void* ptr = sbrk(0);
    sbrk(size_of_block);

    return ptr;
}