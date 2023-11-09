#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdint.h>
#include <stddef.h>

typedef struct memoryData {
    uint32_t free, used, total;
} memoryData;

typedef memoryData * MemoryDataPtr;

typedef struct processInfo {
    char *name;
    int pid;
    int parent;
    uint64_t *rsp; 
    uint64_t *rbp; 
    size_t priority;
    char foreground;
    int status; 
} processInfo;

typedef processInfo * ProcessInfoPtr;

#endif // STRUCTS_H