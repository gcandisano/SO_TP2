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

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <queueADT.h>
#include <process.h>

#define OFF 0
#define ON 1

#define MIN_PRIORITY 1
#define MAX_PRIORITY 5
#define IDLE_PRIORITY 0

extern void forceProcessChange(uint64_t * rsp);

void startScheduler();

void startShell();

uint64_t * scheduler(uint64_t * rsp);

uint8_t getSchedulerStatus();

void stopProcess(uint64_t *stackPointer);

uint64_t * changeProcess(uint64_t * rsp);

uint8_t isReady(void * process);

struct PCB * getNextProcess();

uint8_t shouldChange();

void addProcess(struct PCB * process);

void removeProcess(struct PCB * process);

uint8_t isPid(void * process);

struct PCB * findPcb(int pid);

uint64_t getCurrentPID();

struct PCB * getCurrentPCB();

int changePriority(int pid, int newPriority);

struct PCB ** getChildren(QueueADT queue, int parentPid);

QueueADT * getQueues();

struct PCB * getIdleProcess();

#endif