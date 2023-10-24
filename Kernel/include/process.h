#ifndef PROCESS_H
#define PROCESS_H

#include <stdint.h>
#include <stddef.h>
#include <queueADT.h>
#include <scheduler.h>
#include <lib.h>

#define READY 0
#define BLOCKED 1
#define RUNNING 2
#define ZOMBIE 3
#define DEAD 4

#define READ_FD 0
#define WRITE_FD 1
#define ERROR_FD 2
#define SHELL 0

typedef struct memoryBlock
{
    uint64_t *base;
    size_t size;
    uint64_t *current;
} memoryBlock;


typedef struct PCB
{
    size_t priority;
    size_t ticks;

    int pid;
    char *name;
    int parent;
    memoryBlock *heap;
    memoryBlock *stack;
    int status;
    int return_value;
    int fd[3];
    char foreground;
    int semId;

} PCB;

typedef struct processInfo
{
    char *name;
    int pid;
    int parent;
    uint64_t *rsp; 
    uint64_t *rbp; 
    size_t priority;
    char foreground;
    int status; 
} processInfo;

int createProcess(char *name, int parent, size_t heapSize, size_t stackSize, char **args, void *code, char foreground, int *fds);
//int getNewPid();
int killProcess(int pid);
int killChildren(int parentPid);
int blockProcess(int pid);
int unblockProcess(int pid);
void setFileDescriptor(int pid, int index, int value);
processInfo *getProcessInfo(int pid);
void freeProcess(PCB *processPcb);

#endif