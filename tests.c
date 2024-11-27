#include "tests.h"

int rand_between(int min, int max) {
    return rand() % (max - min + 1) + min;
}

#define TEST_COUNT 20

void test_small_memory_size() {
    printf("Testing small memory size...\n");
    char *ptrs[TEST_COUNT]; // Large enough array to hold many pointers
    int ix = 0;
    // int total_allocated = 0;
    int max_memory = 256;

    // Allocating memory
    for(int ix = 0; ix < TEST_COUNT; ix++) {
        int size = rand_between(1, 20); // Random size between 1 and 20
        // if (total_allocated + size > max_memory) {
        //     size = max_memory - total_allocated; // Adjust size to not exceed max_memory
        // }
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = malloc(size);
        if (ptrs[ix] == NULL) {
            fprintf(stderr, "malloc failed at allocation %d\n", ix);
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n", ix, ix, ptrs[ix], len_to_copy);
        
        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        // fprintf(stderr, "[%d] '%s'\n", ix, ptrs[ix]);
        // total_allocated += size;

        fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);
    }

    // Freeing memory
    for (int ix = 0; ix < TEST_COUNT; ix++) {
        if (ptrs[ix] != NULL) {
            fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
            free(ptrs[ix]);
            fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
        }
    }

    // Testing realloc
    printf("\n\nNow trying to reallocate the buffer...\n");

    printf("Pre realloc...\n");
    char *buffer;

    int size = rand_between(1, 30);
    printf("size: %d\n", size);

    buffer = malloc(size);

    if (buffer == NULL) {
        printf("malloc failed\n");
        exit(1);
    }

    int len_to_copy = MIN(strlen(test_string), size - 1);

    strncpy(buffer, test_string, len_to_copy);
    buffer [len_to_copy] = '\0';

    printf("'%s'\n", buffer);

    printf("\nNow trying to reallocate the memory...\n");

    char *new_buffer = realloc(buffer, (len_to_copy * 2 + 1) * sizeof(char));
    if(new_buffer == NULL) {
        perror("realloc failed FAILED");
        free(buffer);
        exit(1);
    }
    printf("Rellocation: %p\n", new_buffer);

    len_to_copy *= 2;

    printf("Post realloc...\n");
    printf("size: %d\n", len_to_copy);

    strncpy(new_buffer, test_string, len_to_copy);
    new_buffer [len_to_copy] = '\0';

    printf("'%s'\n", new_buffer);

    free(new_buffer);

    printf("\n\nTEST 'test_small_memory_size' PASSED\n\n");
}

void test_large_memory_size() {
    printf("Testing large memory size...\n");
    char *ptrs[TEST_SIZE];
    int ix = 0;

    while (ix < TEST_SIZE) {
        int size = rand_between(1000, 2000); // Large memory size
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = malloc(size);
        if (ptrs[ix] == NULL) {
            fprintf(stderr, "malloc failed\n\nFAIL");
            exit(1);
        }

        int len_to_copy = MIN(strlen("large test"), size - 1);
        strncpy(ptrs[ix], "large test", len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        fprintf(stderr, "[%d] '%s'\n", ix, ptrs[ix]);
        ix++;
    }

    ix = 0;
    while (ix < TEST_SIZE) {
        fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
        free(ptrs[ix]);
        fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
        ix++;
    }

    printf("\n\nTEST 'test_large_memory_size' PASSED\n\n");
}

void test_normal_program() {
    printf("Testing normal program...\n");
    char *test_string = "Now is the time for all good people to come to the aid of their country and the string wasn't long enough for the tests so here's some more stuff to make it longer";
    char *ptrs[TEST_SIZE];
    int ix = 0;

    while (ix < TEST_SIZE) {
        int size = rand_between(1, 30);
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = malloc(size);
        if (ptrs[ix] == NULL) {
            fprintf(stderr, "malloc failed\n\nFAIL");
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);
        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        fprintf(stderr, "[%d] '%s'\n", ix, ptrs[ix]);
        ix++;
    }

    ix = 0;
    while (ix < TEST_SIZE) {
        fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
        free(ptrs[ix]);
        fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
        ix++;
    }

    printf("\n\nTEST 'test_normal_program' PASSED\n\n");
}

void test_interspersed_free() {
    printf("Testing interspersed free()...\n");
    char *test_string = "Interspersed free test";
    char *ptrs[TEST_SIZE];
    int ix = 0;

    while (ix < TEST_SIZE) {
        int size = rand_between(1, 30);
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = malloc(size);
        if (ptrs[ix] == NULL) {
            fprintf(stderr, "malloc failed\n\nFAIL\n");
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);
        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        fprintf(stderr, "[%d] '%s'\n", ix, ptrs[ix]);

        // Free every other allocation
        if (ix % 2 == 0) {
            fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
            free(ptrs[ix]);
            fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
        }

        ix++;
    }

    // Free remaining allocations
    ix = 1;
    while (ix < TEST_SIZE) {
        if (ix % 2 != 0) {
            fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
            free(ptrs[ix]);
            fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
        }
        ix++;
    }

    printf("\n\nTEST 'test_interspersed_free' PASSED\n\n");
}