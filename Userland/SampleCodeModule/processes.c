#include <commands.h>

int processesInfo() {
	ProcessInfoPtr * processesInfo = sys_processes_info();
	if (processesInfo == NULL) {
		printfColor("\n\nNo processes running\n", YELLOW);
		return 1;
	}
	printfColor("\n\nProcesses info:\n", YELLOW);
	int i = 0;
	printColor("  PID\tPriority\tParent\tForeground\tStatus\t   RSP   \t RBP\t   Name\n", CYAN);
	while (processesInfo[i] != NULL) {
		printf("   %d", processesInfo[i]->pid);
		printf(processesInfo[i]->pid > 9 ? "    \t%d" : "     \t%d", processesInfo[i]->priority);
		printf("\t\t %d", processesInfo[i]->parent);
		printf("\t\t  %d", processesInfo[i]->foreground);
		switch (processesInfo[i]->status) {
			case READY:
				printf("\t    Ready ");
				break;
			case BLOCKED:
				printf("\t   Blocked");
				break;
			case RUNNING:
				printf("\t   Running");
				break;
			case ZOMBIE:
				printf("\t Zombie(%d)", processesInfo[i]->exitCode);
				break;
			case DEAD:
				printf("\t   Dead(%d)", processesInfo[i]->exitCode);
				break;
			default:
				printf("\t   Unknown");
				break;
		}
		printf("\t%x", processesInfo[i]->rsp);
		printf("   %x", processesInfo[i]->rbp);
		printf("\t%s\n", processesInfo[i]->name);
		i++;
	}
	return 0;
}


int killProcess(char ** argv) {
	if (argv[1] == 0) {
		printfColor("\n\nNo process id specified\n", RED);
		return 1;
	}
	int zero = 0;
	int pid = strtoi(argv[1], &zero);
	int response = sys_kill_process(pid);
	if (response != 0) {
		printfColor("\n\nProcess %d not found\n", RED, pid);
		return 1;
	}
	return 0;
}