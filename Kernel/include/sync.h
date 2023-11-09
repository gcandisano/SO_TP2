#ifndef SYNC_H
#define SYNC_H

#include <mm_manager.h>
#include <process.h>
#include <queueADT.h>
#include <scheduler.h>

#define MAX_SEMAPHORES       200
#define FIRST_USER_SEMAPHORE 100

typedef int sem_t;

typedef struct TSem {
	char * name;
	size_t value;
	QueueADT blockedProcesses;
	bool activeProcesses[MAX_PROCESSES];  // Keeps track of the processes that have permission to access the semaphore
	size_t activeProcessDim;
	bool destroying;
} TSem;

typedef struct TMutex {
	QueueADT blockedProcesses;
	char value;
	int currentOwnerPID;
} TMutex;

void semInit();

sem_t semCreate(char * name, int initValue);

int semDestroy(sem_t id);

int userExists(char * name);

sem_t semOpen(char * semName);

sem_t semClose(sem_t sem);

sem_t semCreateAnon(int initVal);

sem_t semAnonOpen(sem_t sem);

sem_t findMinFreeSemID();

int semWait(sem_t sem);

int semPost(sem_t sem);

int semSet(int semId, int value);

extern int enterCritRegion(char * semValue);

// MUTEX

void initMutex();

void lockMutex(int mutexID);

void unlockMutex(int mutexID);

void createMutex(int mutexID);

void delteMutex(int mutexID);

#endif