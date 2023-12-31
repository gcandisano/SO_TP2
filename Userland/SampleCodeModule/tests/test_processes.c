// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "test_util.h"

#include <colors.h>
#include <userio.h>
#include <usyscalls.h>

enum State { RUNNING, BLOCKED, KILLED };

typedef struct P_rq {
	int32_t pid;
	enum State state;
} p_rq;

int64_t test_processes(char * argv[]) {
	uint8_t rq;
	uint8_t alive = 0;
	uint8_t action;
	uint64_t max_processes;
	int32_t totalProcesses = 0;
	char * argvAux[] = {0};
	int fds[2] = {0, 1};

	if (argv[1] == 0) {
		printfColor("\nProcess amount required\n", RED);
		return -1;
	}

	if ((max_processes = satoi(argv[1])) <= 0)
		return -1;

	p_rq p_rqs[max_processes];

	while (1) {
		// Create max_processes processes
		for (rq = 0; rq < max_processes; rq++) {
			p_rqs[rq].pid = sys_create_process("endless_loop", argvAux, &endless_loop, 0, fds);
			totalProcesses++;

			if (p_rqs[rq].pid <= 0) {
				printf("test_processes: ERROR creating process %d (%d)\n", totalProcesses, p_rqs[rq].pid);
				return -1;
			} else {
				p_rqs[rq].state = RUNNING;
				alive++;
			}
		}

		// Randomly kills, blocks or unblocks processes until every one has been killed
		while (alive > 0) {
			for (rq = 0; rq < max_processes; rq++) {
				action = GetUniform(100) % 2;

				switch (action) {
					case 0:
						if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED) {
							if (sys_kill_process(p_rqs[rq].pid) != 0) {
								printf("test_processes: ERROR killing process\n");
								return -1;
							}
							sys_kill_process(p_rqs[rq].pid);
							p_rqs[rq].state = KILLED;
							alive--;
						}
						break;

					case 1:
						if (p_rqs[rq].state == RUNNING) {
							if (sys_block_process(p_rqs[rq].pid) != 0) {
								printf("test_processes: ERROR blocking process\n");
								return -1;
							}
							p_rqs[rq].state = BLOCKED;
						}
						break;
				}
			}

			// Randomly unblocks processes
			for (rq = 0; rq < max_processes; rq++)
				if (p_rqs[rq].state == BLOCKED && GetUniform(100) % 2) {
					if (sys_unblock_process(p_rqs[rq].pid) != 0) {
						printf("test_processes: ERROR unblocking process\n");
						return -1;
					}
					p_rqs[rq].state = RUNNING;
				}
		}
	}
}
