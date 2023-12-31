#ifndef STRUCTS_H
#define STRUCTS_H

#include <stddef.h>
#include <stdint.h>

typedef struct memoryData {
	uint32_t free, used, total;
} memoryData;

typedef memoryData * MemoryDataPtr;

typedef struct processInfo {
	char * name;
	int pid;
	int parent;
	uint64_t * rsp;
	uint64_t * rbp;
	size_t priority;
	char foreground;
	int status;
	int exitCode;
} processInfo;

typedef processInfo * ProcessInfoPtr;

#endif  // STRUCTS_H