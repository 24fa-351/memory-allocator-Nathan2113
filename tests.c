#include "tests.h"

int rand_between(int min, int max) {
    return rand() % (max - min + 1) + min;
}

void test_small_memory_size() {
    printf("Testing small memory size...\n");
    char *ptrs[TEST_SIZE];
    int ix = 0;

    while (ix < TEST_SIZE) {
        int size = rand_between(1, 10); // Small memory size
        fprintf(stderr, "[%d] size: %d\n", ix, size);

        ptrs[ix] = malloc(size);
        if (ptrs[ix] == NULL) {
            fprintf(stderr, "malloc failed\n\nFAIL");
            exit(1);
        }

        int len_to_copy = MIN(strlen("small test"), size - 1);
        strncpy(ptrs[ix], "small test", len_to_copy);
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
}