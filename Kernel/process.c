#include <process.h>

int biggerPid = 1;

int createProcess(char *name, int parent, size_t heapSize, size_t stackSize, char **args, void *code, char foreground, int *fds){
    if (name == NULL || code == NULL || heapSize < 0 || stackSize < 0 || fds == NULL) return -1;
    PCB *pcb = (PCB *) malloc(sizeof(PCB));
    if (pcb == NULL) return -1;
    pcb->name = (char *) malloc(strlen(name) + 1);
    if (pcb->name == NULL){
        free(pcb);
        return -1;
    }
    pcb->pid = biggerPid++;
    strcpy(pcb->name, name);
    if (parent == -1){
        pcb->parent = getCurrentPID();//habria que crearla 
    } else {
        pcb->parent = parent;
    }
    pcb->heap= malloc(sizeof(memoryBlock));
    if (pcb->heap == NULL){
        free(pcb->name);
        free(pcb);
        return -1;
    }
    pcb->heap->base = malloc(heapSize);
    if(pcb->heap->base == NULL){
        free(pcb->heap);
        free(pcb->name);
        free(pcb);
        return -1;
    }
    pcb->heap->size = heapSize;
    pcb->heap->current = pcb->heap->base;
    pcb->stack = (memoryBlock *) malloc(sizeof(memoryBlock));
    if(pcb->stack == NULL)
    {
        free(pcb->heap->base);
        free(pcb->heap);
        free(pcb->name);
        free(pcb);
        return -1;
    }
    pcb->stack->base = (uint64_t *)malloc(stackSize);
    if (pcb->stack->base == NULL)
    {
        free(pcb->stack);
        free(pcb->heap->base);
        free(pcb->heap);
        free(pcb->name);
        free(pcb);
        return -1;
    }
    pcb->stack->size = stackSize;
    pcb->stack->current = pcb->stack->base + pcb->stack->size - 1; 
    pcb->status = READY;

    pcb->fd[READ_FD] = fds[READ_FD]; 
    pcb->fd[WRITE_FD] = fds[WRITE_FD]; 
    pcb->fd[ERROR_FD] = fds[ERROR_FD]; 
    pcb->foreground = foreground;
    pcb->stack->current = createStack((uint64_t *)pcb->stack->base + pcb->stack->size, code, args);
    addProcess(pcb);//deberia estar en el scheduler
    return pcb->pid;
}

int killProcess(int pid)
{
    PCB *pcb = findPcb(pid);//scheduler
    if(pcb == NULL) return -1;
    if(findPcb(pcb->parent) == NULL || pcb->status == ZOMBIE) {
        pcb->status = DEAD;
        removeProcess(pcb); // en scheduler
        freeProcess(pcb);
    }
    else{
        pcb->status = ZOMBIE;
    }
    if (pid == getCurrentPID()) {
        forceScheduler();
    }
    return 0;
}

int killChildren(int parentPid) {
    int count = 0;
    QueueADT **myQueues = getQueues();
    for (int i = MIN_PRIORITY; i <= MAX_PRIORITY; i++) {
        int *childrenPids = getChildrenPids(myQueues[i], parentPid);
        for (int j = 0; childrenPids[j] != -1; j++) {
            killProcess(childrenPids[j]);
            count++;
        }
        free(childrenPids);
    }
    return count;
}

int blockProcess(int pid) {
    PCB *pcb = findPcb(pid);
    if(pcb == NULL) return -1;
    pcb->status = BLOCKED;
    if (pid == getCurrentPID()) {
        forceScheduler();
    }
    return 0;
}

int unblockProcess(int pid) {
    PCB *pcb = findPcb(pid);
    if(pcb == NULL) return -1;
    pcb->status = READY;
    return 0;
}


void setFileDescriptor(int pid, int index, int value) {
    findPcb(pid)->fd[index] = value;
}

processInfo *getProcessInfo(int pid)
{
    PCB *pcb = findPcb(pid);
    processInfo *info = malloc(sizeof(processInfo)); 
    if (info == NULL) return NULL;
    info->pid = pcb->pid;
    info->name = malloc(strlen(pcb->name) + 1);
    if (info->name == NULL) { 
        free(info);
        return NULL;
    }
    strcpy(info->name, pcb->name); 
    info->parent = pcb->parent;
    info->status = pcb->status;
    info->priority = pcb->priority;
    info->foreground = pcb->foreground;
    info->rsp = pcb->stack->current; 
    info->rbp = pcb->stack->base; 
    return info;
}

void freeProcess(PCB *pcb)
{
    free(pcb->stack->base);
    free(pcb->stack);
    free(pcb->heap->base);
    free(pcb->heap);
    free(pcb->name);
    free(pcb);
}


