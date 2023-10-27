#ifndef MM_MANAGER_H
#define MM_MANAGER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

void startMemoryManager(const void * start_address, uint64_t size);

void * malloc(uint64_t size);

void free(void * address);

#endif