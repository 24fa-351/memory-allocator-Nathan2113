#include "tests.h"


int main(int argc, char*argv[]) {
    
    srand(time(NULL));

    #ifndef SYSTEM_MALLOC
    initialize_memory_pool();
    #endif

    test_small_memory_size();
    test_large_memory_size();
    test_normal_program();
    test_interspersed_free();

    return 0;
}