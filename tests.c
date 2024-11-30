#include "malloc.c"

void small_test();
void large_test();
void same_size_test();
void interspersed_frees_test();

static char *test_string = "Now is the time for all good people to come to the aid of their country.";
#define TEST_SIZE 10

int rand_between(int min, int max) {
    return rand() % (max - min + 1) + min;
}


#define MIN(a, b) ((a) < (b) ? (a) : (b))

void small_test() {
    #ifndef SYSTEM_MALLOC
    initialize_memory_pool();
    #endif

    printf("Small test\n");
    char *ptrs[TEST_SIZE];

    // Allocating memory
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
        int size = 72;
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