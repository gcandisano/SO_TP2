#ifndef TESTS_H
#define TESTS_H

#include <stdint.h>

uint64_t test_mm(char * argv[]);

void test_prio();

int64_t test_processes(char * argv[]);

uint64_t test_sync(char * argv[]);  //{n, use_sem, 0}

#endif