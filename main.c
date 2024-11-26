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


int rand_between(int min, int max) {
    return rand() % (max - min + 1) + min;
}

#define TEST_SIZE 10 

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int main(int argc, char*argv[]) {
    srand(time(NULL));

    char *test_string = "Now is the time for all good people to come to the aid of their country and the string wasn't long enough for the tests so here's some more stuff to make it longer";

    if(argc > 1) {
        test_string = argv[1];
    }

    char *ptrs[TEST_SIZE];

    for (int ix = 0; ix < TEST_SIZE; ix++) {
        int size = rand_between(1, 30);
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

    for (int ix = 0; ix < TEST_SIZE; ix++) {
        fprintf(stderr, "[%d] freeing %p (%s)\n", ix, ptrs[ix], ptrs[ix]);
        xfree(ptrs[ix]);
        fprintf(stderr, "[%d] freed %p\n", ix, ptrs[ix]);
    }

    printf("\n\nNow trying to reallocate the buffer...\n");

    printf("Pre realloc...\n");
    char *buffer;

    int size = rand_between(1, 30);
    printf("size: %d\n", size);

    buffer = xmalloc(size);

    if (buffer == NULL) {
        printf("malloc failed\n");
        exit(1);
    }

    int len_to_copy = MIN(strlen(test_string), size - 1);

    strncpy(buffer, test_string, len_to_copy);
    buffer [len_to_copy] = '\0';

    printf("'%s'\n", buffer);

    printf("\nNow trying to reallocate the memory...\n");

    char *new_buffer = xrealloc(buffer, (len_to_copy * 2 + 1) * sizeof(char));
    if(new_buffer == NULL) {
        perror("realloc failed");
        free(buffer);
        return 1;
    }
    printf("Rellocation: %p\n", new_buffer);

    len_to_copy *= 2;

    printf("Post realloc...\n");
    printf("size: %d\n", len_to_copy);

    strncpy(new_buffer, test_string, len_to_copy);
    new_buffer [len_to_copy] = '\0';

    printf("'%s'\n", new_buffer);


    return 0;
}