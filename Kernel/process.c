// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <process.h>
#include <sync.h>

int biggerPid = 1;
size_t cantProcesses = 0;

int createProcess(char * name, int parent, size_t stackSize, char ** args, void * code, char foreground, int * fds) {
	if (name == NULL || code == NULL || stackSize <= 0 || fds == NULL)
		return -1;
	PCB * pcb = (PCB *) malloc(sizeof(PCB));
	if (pcb == NULL)
		return -2;
	pcb->name = (char *) malloc(strlen(name) + 1);
	if (pcb->name == NULL) {
		free(pcb);
		return -3;
	}
	pcb->pid = biggerPid++;
	strcpy(pcb->name, name);
	if (parent == -1) {
		pcb->parent = getCurrentPID();
	} else {
		pcb->parent = parent;
	}
	pcb->stack = (memoryBlock *) malloc(sizeof(memoryBlock));
	if (pcb->stack == NULL) {
		free(pcb->name);
		free(pcb);
		return -4;
	}
	pcb->stack->base = (uint64_t *) malloc(stackSize);
	if (pcb->stack->base == NULL) {
		free(pcb->stack);
		free(pcb->name);
		free(pcb);
		return -5;
	}
	pcb->stack->size = stackSize;
	pcb->stack->current = pcb->stack->base + pcb->stack->size - 1;
	pcb->status = READY;

	pcb->fd[READ_FD] = fds[READ_FD];
	pcb->fd[WRITE_FD] = fds[WRITE_FD];
	pcb->foreground = foreground;
	pcb->stack->current = createStack((uint64_t *) pcb->stack->base + pcb->stack->size, code, args, &processWrapper);
	pcb->semId = semCreateAnon(0);
	pcb->exitCode = 0;

	cantProcesses++;

	addProcess(pcb);
	return pcb->pid;
}

void processWrapper(int code(char ** args), char ** args) {
	PCB * currentPcb = getCurrentPCB();
	semAnonOpen(currentPcb->semId);
	currentPcb->exitCode = code(args);
	killProcess(currentPcb->pid);
}

int killProcess(int pid) {
	PCB * pcb = findPcb(pid);
	if (pcb == NULL)
		return 1;

	if (pcb->fd[WRITE_FD] != WRITE_FD) {
		char eof[1] = {-1};
		writePipe(pcb->fd[WRITE_FD], eof, 1);
	}

	if (findPcb(pcb->parent) == NULL || pcb->status == ZOMBIE) {
		pcb->status = DEAD;
		removeProcess(pcb);
		freeProcess(pcb);
		cantProcesses--;
	} else {
		pcb->status = ZOMBIE;
		semPost(pcb->semId);
		semClose(pcb->semId);
	}
	if (pid == getCurrentPID())
		forceScheduler();
	return 0;
}

int killForeground(int sem) {
	PCB * pcb = findPcb(getForegroundProcess());
	if (pcb == NULL)
		return 1;
	killChildren(pcb->pid);
	if (pcb->status == BLOCKED)
		semSet(sem, 1);
	return killProcess(pcb->pid);
}

int killChildren(int parentPid) {
	int count = 0;
	QueueADT * myQueues = getQueues();
	for (int i = MIN_PRIORITY; i <= MAX_PRIORITY; i++) {
		PCB ** children = (PCB **) getChildren(myQueues[i], parentPid);
		for (int j = 0; children[j] != NULL; j++) {
			killProcess(children[j]->pid);
			count++;
		}
		free(children);
	}
	return count;
}

int blockProcess(int pid) {
	PCB * pcb = findPcb(pid);
	if (pcb == NULL)
		return -1;
	pcb->status = BLOCKED;
	if (pid == getCurrentPID()) {
		forceScheduler();
	}
	return 0;
}

int unblockProcess(int pid) {
	PCB * pcb = findPcb(pid);
	if (pcb == NULL)
		return -1;
	pcb->status = READY;
	return 0;
}

void setFileDescriptor(int pid, int index, int value) {
	findPcb(pid)->fd[index] = value;
}

processInfo * getProcessInfo(PCB * pcb) {
	processInfo * info = malloc(sizeof(processInfo));
	if (info == NULL)
		return NULL;
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
	info->exitCode = pcb->exitCode;
	return info;
}

processInfo ** getProcessesInfo() {
	processInfo ** info = malloc(sizeof(processInfo *) * (cantProcesses + 1));
	if (info == NULL)
		return NULL;

	int i = 0;
	QueueADT * myQueues = getQueues();
	for (int j = 0; j <= MAX_PRIORITY; j++) {
		PCB ** processes = (PCB **) getAllElements(myQueues[j]);
		for (int k = 0; processes[k] != NULL; k++) {
			info[i++] = getProcessInfo(processes[k]);
		}
		free(processes);
	}
	info[i] = NULL;
	return info;
}

void freeProcessesInfo(processInfo ** info) {
	for (int i = 0; info[i] != NULL; i++) {
		free(info[i]->name);
		free(info[i]);
	}
	free(info);
}

void freeProcess(PCB * pcb) {
	free(pcb->stack->base);
	free(pcb->stack);
	free(pcb->name);
	semDestroy(pcb->semId);
	free(pcb);
}

int waitpid(int pid) {
	PCB * process = findPcb(pid);

	if (process == NULL) {
		return -1;
	}

	semWait(process->semId);

	killProcess(process->pid);
	return pid;
}

void resetProcesses() {
	cantProcesses = 0;
	biggerPid = 1;
}