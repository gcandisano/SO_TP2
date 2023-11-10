// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_util.h"

#include <stdint.h>
#include <userio.h>
#include <usyscalls.h>

#define MINOR_WAIT 1000000  // TODO: Change this value to prevent a process from flooding the screen
#define WAIT \
	10000000  // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST          1  // TODO: Change as required
#define MEDIUM          3  // TODO: Change as required
#define HIGHEST         5  // TODO: Change as required

int64_t prio[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio() {
	int64_t pids[TOTAL_PROCESSES];
	char * argv[] = {0};
	uint64_t i;
	int fds[3] = {0, 1, 0};

	for (i = 0; i < TOTAL_PROCESSES; i++)
		pids[i] = sys_create_process("endless_loop_print", argv, &endless_loop_print, 0, fds);

	bussy_wait(WAIT);
	printf("\nCHANGING PRIORITIES...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		int r = sys_change_priority(pids[i], prio[i]);
		if (r != 0)
			printf("test_prio: ERROR: sys_change_priority");
	}

	bussy_wait(WAIT);
	printf("\nBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		int r = sys_block_process(pids[i]);
		if (r != 0)
			printf("test_prio: ERROR: sys_block_process");
	}

	printf("CHANGING PRIORITIES WHILE BLOCKED...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		int r = sys_change_priority(pids[i], MEDIUM);
		if (r != 0)
			printf("test_prio: ERROR: sys_change_priority");
	}

	printf("UNBLOCKING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		int r = sys_unblock_process(pids[i]);
		if (r != 0)
			printf("test_prio: ERROR: sys_unblock_process");
	}

	bussy_wait(WAIT);
	printf("\nKILLING...\n");

	for (i = 0; i < TOTAL_PROCESSES; i++) {
		int r1 = sys_kill_process(pids[i]);
		int r2 = sys_kill_process(pids[i]);
		if (r1 != 0 || r2 != 0)
			printf("test_prio: ERROR: sys_kill_process");
	}

	sys_exit(0);
}
