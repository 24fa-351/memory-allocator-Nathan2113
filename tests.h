#ifndef TESTS_H
#define TESTS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "malloc.h"

#define TEST_SIZE 10
#define MIN(a, b) ((a) < (b) ? (a) : (b))

static char *test_string = "Now is the time for all good people to come to the aid of their country";

void test_small_memory_size();
void test_large_memory_size();
void test_normal_program();
void test_interspersed_free();

#endif