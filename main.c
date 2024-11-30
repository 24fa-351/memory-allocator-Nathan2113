#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// define "x" for system malloc, include for our versions. Don't do both
#ifdef SYSTEM_MALLOC
#define xfree free
#define xmalloc malloc
#define xrealloc realloc
#else
#include "malloc.h"
#endif

static char *test_string = "Now is the time for all good people to come to the aid of their country.";


int rand_between(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void small_test();
void large_test();
void same_size_test();
void interspersed_frees_test();

#define TEST_SIZE 10

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char*argv[]) {
    srand(time(NULL));

    #ifndef SYSTEM_MALLOC
    initialize_memory_pool();
    #endif

    // Check if the user provided the "-t" flag
    if (argc == 3 && strcmp(argv[1], "-t") == 0) {
        int test_case = atoi(argv[2]);

        switch (test_case) {
            case 1:
                small_test();
                break;
            case 2:
                large_test();
                break;
            case 3: 
                same_size_test();
                break;
            case 4: 
                interspersed_frees_test();
                break;
            default:
                fprintf(stderr, "Invalid test case number\n");
                return 1;
        }
    }
    else {
        small_test();
        large_test();
        same_size_test();
        interspersed_frees_test();
    }


    return 0;
}


void small_test() {
    #ifndef SYSTEM_MALLOC
    initialize_memory_pool();
    #endif

    printf("Small test\n");
    char *ptrs[TEST_SIZE];

    // Allocating memory
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        int size = rand_between(1, 10);
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = xmalloc(size);
        if (ptrs[ix] == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n",
            ix, ix, ptrs[ix], len_to_copy);
        
        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix] [len_to_copy] = '\0';

        fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);
    }

    // Freeing memory
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
        xfree(ptrs[ix]);
        fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
    }

    printf("\nTEST small_test PASSED\n\n");
}

void large_test() {
    #ifndef SYSTEM_MALLOC
    initialize_memory_pool();
    #endif

    printf("Large test\n");
    char *ptrs[TEST_SIZE];

    // Allocating memory
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        int size = rand_between(10, 72);
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = xmalloc(size);
        if (ptrs[ix] == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n",
            ix, ix, ptrs[ix], len_to_copy);
        
        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix] [len_to_copy] = '\0';

        fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);
    }

    // Freeing memory
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
        xfree(ptrs[ix]);
        fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
    }

    printf("\nTEST large_test PASSED\n\n");
}


void same_size_test() {
    #ifndef SYSTEM_MALLOC
    initialize_memory_pool();
    #endif

    printf("Same size test\n");
    char *ptrs[TEST_SIZE];

    // Allocating memory
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        int size = 50;
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = xmalloc(size);
        if (ptrs[ix] == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n",
            ix, ix, ptrs[ix], len_to_copy);
        
        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix] [len_to_copy] = '\0';

        fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);
    }

    // Freeing memory
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
        xfree(ptrs[ix]);
        fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
    }

    printf("\nTEST same_size_test PASSED\n\n");
}


void interspersed_frees_test() {
    #ifndef SYSTEM_MALLOC
    initialize_memory_pool();
    #endif

    printf("Interspersed frees test\n");
    char *ptrs[TEST_SIZE];

    // Allocating and freeing memory interspersed
    for (int ix = 0; ix < TEST_SIZE; ix++) {
        int size = rand_between(1, 72);
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = xmalloc(size);
        if (ptrs[ix] == NULL) {
            fprintf(stderr, "malloc failed\n");
            exit(1);
        }

        int len_to_copy = MIN(strlen(test_string), size - 1);

        fprintf(stderr, "[%d] ptrs[%d]: %p, going to copy %d chars\n",
            ix, ix, ptrs[ix], len_to_copy);
        
        strncpy(ptrs[ix], test_string, len_to_copy);
        ptrs[ix][len_to_copy] = '\0';

        fprintf(stderr, "[%x] '%s'\n", ix, ptrs[ix]);

        // Free memory after each allocation
        fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
        xfree(ptrs[ix]);
        fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
    }

    printf("\nTEST interspersed_frees_test PASSED\n\n");
}