#ifndef MM_MANAGER_H
#define MM_MANAGER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct node {
	void * address;
	uint64_t size;
	bool is_free;
	struct node * prev;
	struct node * next;
};

typedef struct node * NodePtr;

typedef struct memoryData {
	uint32_t free, used, total;
} memoryData;

typedef memoryData * MemoryDataPtr;

void startMemoryManager(const void * start_address, uint64_t size);

void * malloc(uint64_t size);

int free(void * address);

MemoryDataPtr getMemoryData();

#endif