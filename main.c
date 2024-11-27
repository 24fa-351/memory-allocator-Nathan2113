
#include "tests.h"

// #define TEST_SIZE 200

// #define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char*argv[]) {
    
    srand(time(NULL));

    #ifndef SYSTEM_MALLOC
    initialize_memory_pool();
    #endif

    test_small_memory_size();
    // test_large_memory_size();
    // test_normal_program();
    // test_interspersed_free();

    return 0;
    // srand(time(NULL));

    // #ifndef SYSTEM_MALLOC
    // initialize_memory_pool();
    // #endif

    // if(argc > 1) {
    //     test_string = argv[1];
    // }

    // char *ptrs[TEST_SIZE];

    // // Allocating memory
    // for (int ix = 0; ix < TEST_SIZE; ix++) {
    //     int size = rand_between(1, 30);
    //     fprintf(stderr, "[%d] size: %d\n", ix, size);

    //     ptrs[ix] = xmalloc(size);
    //     if (ptrs[ix] == NULL) {
    //         fprintf(stderr, "malloc failed\n");
    //         exit(1);
    //     }

    //     int len_to_copy = MIN(strlen(test_string), size - 1);

    //     fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n",
    //         ix, ix, ptrs[ix], len_to_copy);
        
    //     strncpy(ptrs[ix], test_string, len_to_copy);
    //     ptrs[ix] [len_to_copy] = '\0';

    //     fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);
    // }

    // // Freeing memory
    // for (int ix = 0; ix < TEST_SIZE; ix++) {
    //     fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
    //     xfree(ptrs[ix]);
    //     fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
    // }

    // // Testing realloc
    // printf("\n\nNow trying to reallocate the buffer...\n");

    // printf("Pre realloc...\n");
    // char *buffer;

    // int size = rand_between(1, 30);
    // printf("size: %d\n", size);

    // buffer = xmalloc(size);

    // if (buffer == NULL) {
    //     printf("malloc failed\n");
    //     exit(1);
    // }

    // int len_to_copy = MIN(strlen(test_string), size - 1);

    // strncpy(buffer, test_string, len_to_copy);
    // buffer [len_to_copy] = '\0';

    // printf("'%s'\n", buffer);

    // printf("\nNow trying to reallocate the memory...\n");

    // char *new_buffer = xrealloc(buffer, (len_to_copy * 2 + 1) * sizeof(char));
    // if(new_buffer == NULL) {
    //     perror("realloc failed");
    //     free(buffer);
    //     return 1;
    // }
    // printf("Rellocation: %p\n", new_buffer);

    // len_to_copy *= 2;

    // printf("Post realloc...\n");
    // printf("size: %d\n", len_to_copy);

    // strncpy(new_buffer, test_string, len_to_copy);
    // new_buffer [len_to_copy] = '\0';

    // printf("'%s'\n", new_buffer);


    // return 0;
}