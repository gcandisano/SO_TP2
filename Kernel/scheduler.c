/* El sistema DEBERÁ implementar el siguiente algoritmo de scheduling:
● Priority-based round Robin.
El sistema también DEBERÁ proveer los siguientes servicios:
● Crear y finalizar un proceso. DEBERÁ soportar el pasaje de parámetros.
● Obtener el ID del proceso que llama.
● Listar todos los procesos: nombre, ID, prioridad, stack y base pointer, foreground y
cualquier otra variable que consideren necesaria.
● Matar un proceso arbitrario.
● Modificar la prioridad de un proceso arbitrario.
● Bloquear y desbloquear un proceso arbitrario.
● Renunciar al CPU
● Esperar a que los hijos terminen */

#include <scheduler.h>

uint8_t schedulerStatus = OFF;

uint8_t quantums[5] = {1, 2, 4, 8, 16};

QueueADT * queues[MAX_PRIORITY];

struct PCB * currentProcess;

void startScheduler() {
    schedulerStatus = ON;
    for (int i = 0; i < MAX_PRIORITY; i++) {
        queues[i] = createQueue();
    }
}

uint64_t * scheduler(uint64_t * rsp) {
    if (schedulerStatus == ON) {
        if (shouldChange()) {
            return changeProcess(rsp);
        }
    }
    return NULL;
}

uint8_t getSchedulerStatus() {
    return schedulerStatus;
}

void stopProcess(uint64_t *stackPointer) {
    currentProcess->stack->current = stackPointer;
    if (currentProcess->ticks == quantums[currentProcess->priority]) { // process runed the whole quantum 
        if (currentProcess->priority > MIN_PRIORITY) {
            changePriority(currentProcess->pid, currentProcess->priority - 1); // decrease its priority
        } else {

            removeProcess(currentProcess);
            addProcess(currentProcess);
        }
    } else { // process didnt run the whole quantum
        if (currentProcess->priority < MAX_PRIORITY) {
            int newPriority = currentProcess->priority + (int)(1 / (double)(currentProcess->ticks / quantums[currentProcess->priority]));
            changePriority(currentProcess->pid, newPriority);
        }
        else {
            removeProcess(currentProcess);
            addProcess(currentProcess);
        }
    }
}

uint64_t * changeProcess(uint64_t * rsp) {
    stopProcess(rsp);
    PCB * nextProcess = getNextProcess();
    if (currentProcess->status == RUNNING)
    {
        currentProcess->status = READY; // the process is now ready
        currentProcess->ticks = 0;
    }
    currentProcess = nextProcess;     // the actual process is the one i found
    currentProcess->status = RUNNING; // the process is now running
    return currentProcess->stack->current;
}

uint8_t isRunning(PCB * process) {
    return process->status == RUNNING;
}

PCB * getNextProcess() {
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

void addProcess(PCB * process) {
    enqueue(queues[process->priority], process);
}

void removeProcess(PCB * process) {
    dequeueByData(queues[process->priority], process);
}

uint8_t isPid(PCB * process) {
    return process->pid == getCurrentPID();
}


PCB * findPcb(int pid) {
    PCB *elementToReturn;
    for (int i = MAX_PRIORITY; i >= MIN_PRIORITY; i--) {
        if ((elementToReturn = findElement(queues[i], isPid)) != NULL) {
            return elementToReturn;
        }
    }
    return NULL;
}