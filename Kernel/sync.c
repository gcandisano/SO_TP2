// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <sync.h>

char * anonSemName = "AnonSem";

TSem semaphores[MAX_SEMAPHORES];

void semInit() {
	int i;
	memset(semaphores, 0, sizeof(TSem) * MAX_SEMAPHORES);
	initMutex();
	for (i = 0; i < MAX_SEMAPHORES; i++) {
		createMutex(i);
	}
}

sem_t semCreate(char * name, int initValue) {
	if (userExists(name))
		return -1;

	int i = FIRST_USER_SEMAPHORE;
	while (i < MAX_SEMAPHORES && semaphores[i].name != NULL)
		i++;

	if (i == MAX_SEMAPHORES)
		return -1;

	semaphores[i].name = (char *) malloc(strlen(name) + 1);
	if (semaphores[i].name == NULL)
		return -1;

	strcpy(semaphores[i].name, name);
	semaphores[i].value = initValue;
	semaphores[i].blockedProcesses = createQueue();
	return i;
}

int semDestroy(sem_t id) {
	if (id < 0 || id >= MAX_SEMAPHORES || semaphores[id].name == NULL || semaphores[id].destroying) {
		return -1;
	}

	semaphores[id].destroying = true;
	deleteQueue(semaphores[id].blockedProcesses);
	if (semaphores[id].name != anonSemName)
		free(semaphores[id].name);
	memset(&semaphores[id], 0, sizeof(TSem));  // Clear the semaphore
	return 0;
}

int userExists(char * name) {
	for (int i = FIRST_USER_SEMAPHORE; i < MAX_SEMAPHORES; i++) {
		if (semaphores[i].name != NULL && strcmp(semaphores[i].name, name) == 0) {
			return 1;
		}
	}
	return 0;
}

sem_t semOpen(char * semName) {
	int i = FIRST_USER_SEMAPHORE;
	while (i < MAX_SEMAPHORES && (semaphores[i].name == NULL || strcmp(semaphores[i].name, semName) != 0)) {
		i++;
	}
	if (i == MAX_SEMAPHORES || semaphores[i].destroying) {
		return -1;
	}
	semaphores[i].activeProcesses[getCurrentPID() % MAX_PROCESSES] = 1;
	semaphores[i].activeProcessDim++;
	return i;
}

sem_t semClose(sem_t sem) {
	if (sem < 0 || sem >= MAX_SEMAPHORES || semaphores[sem].name == NULL ||
	    semaphores[sem].activeProcesses[getCurrentPID() % MAX_PROCESSES] == false)
		return -1;
	semaphores[sem].activeProcesses[getCurrentPID() % MAX_PROCESSES] = false;
	semaphores[sem].activeProcessDim--;
	return 0;
}

sem_t semCreateAnon(int initVal) {
	sem_t minFreeSemID = findMinFreeSemID();
	if (minFreeSemID == -1)
		return -1;

	semaphores[minFreeSemID].name = anonSemName;
	semaphores[minFreeSemID].value = initVal;
	semaphores[minFreeSemID].blockedProcesses = createQueue();
	return minFreeSemID;
}

sem_t semAnonOpen(sem_t sem) {
	if (sem < 0 || sem >= MAX_SEMAPHORES || semaphores[sem].name == NULL || semaphores[sem].destroying)
		return -1;
	semaphores[sem].activeProcesses[getCurrentPID() % MAX_PROCESSES] = true;
	semaphores[sem].activeProcessDim++;
	return 0;
}

sem_t findMinFreeSemID() {
	for (int i = 0; i < FIRST_USER_SEMAPHORE; i++) {
		if (semaphores[i].name == NULL)
			return i;
	}
	return -1;
}

int semWait(sem_t sem) {
	if (sem < 0 || sem >= MAX_SEMAPHORES || semaphores[sem].name == NULL)
		return -1;
	lockMutex(sem);
	if (semaphores[sem].value > 0) {
		semaphores[sem].value--;
		unlockMutex(sem);
		return 0;
	} else {
		enqueue(semaphores[sem].blockedProcesses, getCurrentPCB());
		unlockMutex(sem);
		blockProcess(getCurrentPID());
		return 0;
	}
}

int semPost(sem_t sem) {
	if (sem < 0 || sem >= MAX_SEMAPHORES || semaphores[sem].name == NULL)
		return -1;
	lockMutex(sem);
	if (isEmpty(semaphores[sem].blockedProcesses))
		semaphores[sem].value++;
	else if (semaphores[sem].value == 0) {
		PCB * headOfQueue = dequeue(semaphores[sem].blockedProcesses);
		if (headOfQueue != NULL)
			unblockProcess(headOfQueue->pid);
	}
	unlockMutex(sem);
	return 0;
}

int semSet(int semId, int value) {
	if (semId < 0 || semId >= MAX_SEMAPHORES || value < 0) {
		return -1;
	}

	lockMutex(semId);
	if (semaphores[semId].value == 0) {
		unlockMutex(semId);
		for (int i = 0; i < value; i++) {
			semPost(semId);
		}
		return 0;
	}
	semaphores[semId].value = value;
	unlockMutex(semId);
	return 0;
}

// MUTEX

TMutex mutexArray[MAX_SEMAPHORES];

void initMutex() {
	memset(mutexArray, 0, sizeof(TMutex) * MAX_SEMAPHORES);
}

void lockMutex(int id) {
	if (!enterCritRegion(&mutexArray[id].value)) {
		enqueue(mutexArray[id].blockedProcesses, getCurrentPCB());
		blockProcess(getCurrentPID());
	}
}

void unlockMutex(int id) {
	PCB * headOfQueue = dequeue(mutexArray[id].blockedProcesses);
	if (headOfQueue != NULL) {
		unblockProcess(headOfQueue->pid);
	} else {
		mutexArray[id].value = 0;
	}
}

void createMutex(int id) {
	if (id < 0 || id >= MAX_SEMAPHORES) {
		return;
	}
	if (mutexArray[id].blockedProcesses == NULL) {
		mutexArray[id].blockedProcesses = createQueue();
	}
	mutexArray[id].value = 0;
}

void deleteMutex(int id) {
	if (id < 0 || id >= MAX_SEMAPHORES) {
		return;
	}
	deleteQueue(mutexArray[id].blockedProcesses);
}