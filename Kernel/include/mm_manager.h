#ifndef MM_MANAGER_H
#define MM_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint64_t * ptr_t;

void startMemoryManager(uint64_t start_address, uint64_t size);

ptr_t malloc(uint64_t size);

void free(ptr_t address);

#endif