#include "malloc.h"

void* mallc(size_t size) {
    return xmalloc(size);
}

void free(void *ptr) {
    xfree(ptr);
}

void* realloc(void *ptr, size_t size) {
    return xrealloc(ptr, size);
}