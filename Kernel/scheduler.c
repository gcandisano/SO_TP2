/* El sistema DEBERÁ implementar el siguiente algoritmo de scheduling:
● Priority-based round Robin.
El sistema también DEBERÁ proveer los siguientes servicios:
● Crear y finalizar un proceso. DEBERÁ soportar el pasaje de parámetros.
● Obtener el ID del proceso que llama.
● Listar todos los procesos: nombre, ID, prioridad, stack y base pointer,
foreground y cualquier otra variable que consideren necesaria. ● Matar un
proceso arbitrario. ● Modificar la prioridad de un proceso arbitrario. ●
Bloquear y desbloquear un proceso arbitrario. ● Renunciar al CPU ● Esperar a que
los hijos terminen */

#include <scheduler.h>

uint8_t schedulerStatus = OFF;

uint8_t quantums[6] = {1, 16, 8, 4, 2, 1};

QueueADT queues[MAX_PRIORITY];

struct PCB *currentProcess;

void startScheduler() {
  for (int i = 0; i < MAX_PRIORITY; i++) {
    queues[i] = createQueue();
  }
}

void startShell(int pid) {
  currentProcess = findPcb(pid);
  currentProcess->status = RUNNING;
  schedulerStatus = ON;
  forceProcessChange(currentProcess->stack->current);
}

uint64_t *scheduler(uint64_t *rsp) {
  if (schedulerStatus == ON) {
    if (shouldChange()) {
      return changeProcess(rsp);
    }
  }
  return NULL;
}

uint8_t getSchedulerStatus() { return schedulerStatus; }

void stopProcess(uint64_t *stackPointer) {
  currentProcess->stack->current = stackPointer;
  if (currentProcess->ticks ==
      quantums[currentProcess->priority]) { // process runed the whole quantum
    if (currentProcess->priority > MIN_PRIORITY) {
      changePriority(currentProcess->pid,
                     currentProcess->priority - 1); // decrease its priority
    } else {

      removeProcess(currentProcess);
      addProcess(currentProcess);
    }
  } else { // process didnt run the whole quantum
    if (currentProcess->priority < MAX_PRIORITY) {
      int newPriority = currentProcess->priority +
                        (int)(1 / (double)(currentProcess->ticks /
                                           quantums[currentProcess->priority]));
      changePriority(currentProcess->pid, newPriority);
    } else {
      removeProcess(currentProcess);
      addProcess(currentProcess);
    }
  }
}

uint64_t *changeProcess(uint64_t *rsp) {
  stopProcess(rsp);
  PCB *nextProcess = getNextProcess();
  if (currentProcess->status == RUNNING) {
    currentProcess->status = READY; // the process is now ready
    currentProcess->ticks = 0;
  }
  currentProcess = nextProcess;     // the actual process is the one i found
  currentProcess->status = RUNNING; // the process is now running
  return currentProcess->stack->current;
}

uint8_t isRunning(void *process) { return ((PCB *)process)->status == RUNNING; }

PCB *getNextProcess() { // preguntar a emi
  PCB *elementToReturn;
  for (int i = MAX_PRIORITY; i >= MIN_PRIORITY; i--) {
    if ((elementToReturn = findElement(queues[i], isRunning)) != NULL) {
      return elementToReturn;
    }
  }
  if (currentProcess->status == RUNNING) {
    return currentProcess;
  } else {
    return getIdleProcess();
  }
}

uint8_t shouldChange() {
  if (currentProcess->status != RUNNING) {
    return 1;
  }
  if (currentProcess->ticks >= quantums[currentProcess->priority]) {
    return 1;
  } else {
    currentProcess->ticks++;
    return 0;
  }
}

void addProcess(PCB *process) { enqueue(queues[process->priority], process); }

void removeProcess(PCB *process) {
  dequeueByData(queues[process->priority], process);
}

int auxPid;
uint8_t isPid(void *process) { return ((PCB *)process)->pid == auxPid; }

PCB *findPcb(int pid) {
  PCB *elementToReturn;
  auxPid = pid;
  for (int i = MAX_PRIORITY; i >= MIN_PRIORITY; i--) {
    if ((elementToReturn = findElement(queues[i - 1], isPid)) != NULL) {
      return elementToReturn;
    }
  }
  return NULL;
}

uint64_t getCurrentPID() { return currentProcess->pid; }

int changePriority(int pid, int newPriority) {
  PCB *processToChange = findPcb(pid);

  if (processToChange == NULL) {
    return -1;
  }
  dequeueByData(queues[processToChange->priority], processToChange);
  if (newPriority > MAX_PRIORITY) {
    processToChange->priority = MAX_PRIORITY;
  } else if (newPriority <
             MIN_PRIORITY - 1) { //-1 because of IDLE process priority
    processToChange->priority = MIN_PRIORITY;
  } else {
    processToChange->priority = newPriority;
  }
  enqueue(queues[processToChange->priority], processToChange);
  return 0;
}

uint8_t isChild(void *process, int pid) {
  return ((PCB *)process)->parent == pid;
}

int *getChildrenPids(QueueADT queue, int parentPid) {
  return findElements(queue, isChild, parentPid);
}

QueueADT *getQueues() { return queues; }

struct PCB *getIdleProcess() {
  return peek(queues[0]);
}