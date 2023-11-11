// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_util.h"

#include <stdint.h>
#include <userio.h>
#include <usyscalls.h>

#define SEM_ID               "sem"
#define TOTAL_PAIR_PROCESSES 2

int32_t global;  // shared memory

void slowInc(int32_t * p, int64_t inc) {
	uint64_t aux = *p;
	sys_yield();  // This makes the race condition highly probable
	aux += inc;
	*p = aux;
}

uint64_t my_process_inc(char * argv[]) {
	uint64_t n;
	int8_t inc;
	int8_t use_sem;
	int8_t semId;

	if (argv[0] == 0 || argv[1] == 0 || argv[2] == 0)
		return -1;

	if ((n = satoi(argv[0])) <= 0)
		return -1;
	if ((inc = satoi(argv[1])) == 0)
		return -1;
	if ((use_sem = satoi(argv[2])) < 0)
		return -1;

	if (use_sem) {
		if (!sys_sem_create(SEM_ID, 1)) {
			printf("test_sync: ERROR creating semaphore\n");
			return -1;
		}
		if (!(semId = sys_sem_open(SEM_ID))) {
			printf("test_sync: ERROR opening semaphore\n");
			return -1;
		}
	}

	uint64_t i;
	for (i = 0; i < n; i++) {
		if (use_sem)
			sys_sem_wait(semId);
		slowInc(&global, inc);
		if (use_sem)
			sys_sem_post(semId);
	}

	if (use_sem)
		sys_sem_close(semId);

	return 0;
}

uint64_t test_sync(char * argv[]) {  //{n, use_sem, 0}
	uint64_t pids[2 * TOTAL_PAIR_PROCESSES];

	if (argv[1] == 0 || argv[2] == 0)
		return -1;

	char * argvDec[] = {argv[1], "-1", argv[2], NULL};
	char * argvInc[] = {argv[1], "1", argv[2], NULL};
	int fds[2] = {0, 1};

	global = 0;

	uint64_t i;
	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		pids[i] = sys_create_process("my_process_inc", argvDec, &my_process_inc, 0, fds);
		pids[i + TOTAL_PAIR_PROCESSES] = sys_create_process("my_process_inc", argvInc, &my_process_inc, 0, fds);
	}

	for (i = 0; i < TOTAL_PAIR_PROCESSES; i++) {
		sys_wait_pid(pids[i]);
		sys_wait_pid(pids[i + TOTAL_PAIR_PROCESSES]);
	}

	printf("Final value: %d\n", global);

	return 0;
}
