// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <boca.h>
#include <colors.h>
#include <commands.h>
#include <pong.h>
#include <shell.h>
#include <sounds.h>
#include <userio.h>
#include <usyscalls.h>

#define COMMANDS_QUANTITY (sizeof(commandsNames) / sizeof(char *))

static char * commandsNames[] = {"help",     "time",  "date",   "registers", "fillregs", "div0",    "invalidop",
                                 "pong",     "clear", "mem",    "ps",        "kill",     "block",   "unblock",
                                 "yield",    "nice",  "loop",   "cat",       "testmm",   "testpro", "testprio",
                                 "testsync", "wc",    "filter", "phylos"};

static char * commands[] = {
    "\thelp: gives you a list of all existent commands.\n",
    "\ttime: prints the time of the OS.\n",
    "\tdate: prints the date of the OS.\n",
    "\tregisters: print the state of the registers at the time you screenshot them with ALT key.\n",
    "\tfillregs: fill the registers with stepped values for testing.\n",
    "\tdiv0: divide by zero to trigger exception\n",
    "\tinvalidop: trigger invalid operation code exception\n",
    "\tpong: go to play the \"pong\" game.\n",
    "\tclear: clears the OS screen.\n",
    "\tmem: prints memory status.\n",
    "\tps: prints processes info.\n",
    "\tkill: kill processes by pid.\n",
    "\tblock: block processes by pid.\n",
    "\tunblock: unblock processes by pid.\n",
    "\tyield: yield the current process.\n",
    "\tnice: change the priority of a process.\n",
    "\tloop: create a process that loops forever.\n",
    "\tcat: prints what you type.\n",
    "\ttestmm: test memory manager.\n",
    "\ttestpro: test processes.\n",
    "\ttestprio: test priority.\n",
    "\ttestsync: test sync.\n",
    "\twc: prints the number of lines of what you type.\n",
    "\tfilter: prints the vowels of what you type.\n",
    "\tphylos: go to play the \"phylos\" game.\n"};

char * loopArgs[2] = {"loop", NULL};
char * testmmArgs[3];
char * testProArgs[3];
char * testSyncArgs[4];

void shell() {
	printColor("Welcome to HomerOS. Type \"help\" for command list\n", ORANGE);
	printColor("\nHomerOS: $> ", GREEN);

	int count = 0;
	char buffer[1024] = {0};
	char oldBuffer[1024] = {0};
	int defaultFds[2] = {0, 1};
	char flag = 0;  // Used for up arrow
	while (1) {
		unsigned char c = getChar();
		if (c == '\n') {
			buffer[count] = 0;
			analizeBuffer(buffer, count, 0, defaultFds);
			printColor("\nHomerOS: $> ", GREEN);
			strcpy(oldBuffer, buffer);
			flag = 1;
			count = 0;
		} else if (c == '\b') {
			if (count > 0) {
				printChar(c);
				count--;
			}
		} else if (c == '\t') {
			// analize count letters of the buffer and see if match with any command, if so, complete the command
			int i = 0;
			while (i < COMMANDS_QUANTITY && !strncmp(buffer, commandsNames[i], count)) {
				i++;
			}
			if (i < COMMANDS_QUANTITY) {
				while (commandsNames[i][count] != 0) {
					printChar(commandsNames[i][count]);
					buffer[count] = commandsNames[i][count];
					count++;
				}
			}
		} else if (c == 17 && flag) {
			// up arrow
			while (count > 0) {
				printChar('\b');
				count--;
			}
			strcpy(buffer, oldBuffer);
			count = strlen(buffer);
			print(buffer);
			flag = 0;
		} else if (c == 20 && !flag) {
			// down arrow
			while (count > 0) {
				printChar('\b');
				count--;
			}
			flag = 1;
		} else if (c > 20 && c < 127) {
			printChar(c);
			buffer[count++] = c;
			buffer[count] = 0;
		}
	}
}

void printHelp() {
	printColor("\n\nComandos disponibles:\n\n", YELLOW);
	for (int i = 0; i < COMMANDS_QUANTITY; i++) {
		printColor(commands[i], CYAN);
	}
}

int commandMatch(char * str1, char * command, int count) {
	int i = 0;
	while (*str1 == ' ') {
		str1++;
	}
	while (i < count && str1[i] != 0 && command[i] != 0 && str1[i] == command[i]) {
		i++;
		if (str1[i] == ' ') {
			i--;
			break;
		}
	}
	return str1[i] == command[i];
}

int hasPipe(char * buffer) {
	int i = 0;
	while (buffer[i] != 0) {
		if (buffer[i] == '|')
			return 1;
		i++;
	}
	return 0;
}

int isBackground(char * buffer) {
	int i = 0;
	while (buffer[i] != 0) {
		if (buffer[i] == '&') {
			buffer[i] = 0;
			return 1;
		}
		i++;
	}
	return 0;
}

void analyzePipedCommand(char * buffer, int count) {
	char * commands[2];
	int i = 0;
	while (buffer[i] != 0) {
		if (buffer[i] == '|') {
			buffer[i] = 0;
			commands[0] = buffer;
			commands[1] = &(buffer[i + 1]);
			break;
		}
		i++;
	}

	int pipeFd = sys_create_anon_pipe();
	int fds1[2] = {0, pipeFd};
	int fds2[2] = {pipeFd, 1};
	int pid1 = analizeBuffer(commands[0], strlen(commands[0]), 1, fds1);
	int pid2 = analizeBuffer(commands[1], strlen(commands[1]), 1, fds2);

	if (pid1 > 0)
		sys_wait_pid(pid1);
	if (pid2 > 0)
		sys_wait_pid(pid2);

	sys_destroy_pipe(pipeFd);
}

int wait(int pid, int piped, int background) {
	if (!piped && !background)
		sys_wait_pid(pid);
	return pid;
}

int analizeBuffer(char * buffer, int count, short piped, int * fds) {
	if (count <= 0)
		return -1;
	if (hasPipe(buffer)) {
		analyzePipedCommand(buffer, count);
		return 0;
	}
	int background = isBackground(buffer);
	if (commandMatch(buffer, "help", count) || commandMatch(buffer, "HELP", count)) {
		char * args[2] = {"help", NULL};
		int pid = sys_create_process("help", args, &printHelp, !background, fds);
		sys_wait_pid(pid);
		return pid;
	} else if (commandMatch(buffer, "time", count)) {
		char * args[2] = {"time", NULL};
		int pid = sys_create_process("time", args, &timePrint, !background, fds);
		sys_wait_pid(pid);
		return pid;
	} else if (commandMatch(buffer, "date", count)) {
		char * args[2] = {"date", NULL};
		int pid = sys_create_process("date", args, &datePrint, !background, fds);
		sys_wait_pid(pid);
		return pid;
	} else if (commandMatch(buffer, "registers", count)) {
		char * args[2] = {"registers", NULL};
		int pid = sys_create_process("registers", args, &printRegs, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "fillregs", count)) {
		char * args[2] = {"fillregs", NULL};
		int pid = sys_create_process("fillregs", args, &fillRegisters, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "clear", count)) {
		sys_clear_screen();
	} else if (commandMatch(buffer, "pong", count)) {
		char * args[2] = {"pong", NULL};
		int pid = sys_create_process("pong", args, &pong, !background, fds);
		// if (!piped && !background)
		sys_wait_pid(pid);
		return pid;
	} else if (commandMatch(buffer, "div0", count)) {
		divideByZero();
	} else if (commandMatch(buffer, "invalidop", count)) {
		invalidOpcode();
	} else if (commandMatch(buffer, "mem", count)) {
		char * args[2] = {"mem", NULL};
		int pid = sys_create_process("mem", args, &memInfo, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "ps", count)) {
		char * args[2] = {"ps", NULL};
		int pid = sys_create_process("ps", args, &processesInfo, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "kill", count)) {
		char * args[3];
		parseCommand(args, buffer, 3);
		int pid = sys_create_process("kill", args, &killProcess, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "block", count)) {
		char * args[3];
		parseCommand(args, buffer, 3);
		int pid = sys_create_process("block", args, &blockProcess, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "unblock", count)) {
		char * args[3];
		parseCommand(args, buffer, 3);
		int pid = sys_create_process("unblock", args, &unblockProcess, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "yield", count)) {
		char * args[3] = {"yield", NULL};
		int pid = sys_create_process("yield", args, &yield, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "nice", count)) {
		char * args[4];
		parseCommand(args, buffer, 4);
		int pid = sys_create_process("nice", args, &changePriority, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "loop", count)) {
		int pid = sys_create_process("loop", loopArgs, &infiniteLoop, !background, fds);
		if (!background)
			sys_wait_pid(pid);
		return pid;
	} else if (commandMatch(buffer, "testmm", count)) {
		parseCommand(testmmArgs, buffer, 3);
		int pid = sys_create_process("testmm", testmmArgs, &test_mm, !background, fds);
		if (!background)
			sys_wait_pid(pid);
		return pid;
	} else if (commandMatch(buffer, "testpro", count)) {
		parseCommand(testProArgs, buffer, 3);
		int pid = sys_create_process("testpro", testProArgs, &test_processes, !background, fds);
		if (!background)
			sys_wait_pid(pid);
		return pid;
	} else if (commandMatch(buffer, "testprio", count)) {
		int pid = sys_create_process("testprio", NULL, &test_prio, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "testsync", count)) {
		printChar('\n');
		parseCommand(testSyncArgs, buffer, 3);
		int pid = sys_create_process("testsync", testSyncArgs, &test_sync, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "boca", count)) {
		sys_clear_screen();
		sys_draw_image(diego, 100, 100);
		playBSong();
		sys_clear_screen();
	} else if (commandMatch(buffer, "cat", count)) {
		char * args[2] = {"cat", NULL};
		int pid = sys_create_process("cat", args, &cat, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "wc", count)) {
		char * args[2] = {"wc", NULL};
		int pid = sys_create_process("wc", args, &wc, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "filter", count)) {
		char * args[2] = {"filter", NULL};
		int pid = sys_create_process("filter", args, &filter, !background, fds);
		return wait(pid, piped, background);
	} else if (commandMatch(buffer, "phylos", count)) {
		char * args[2] = {"phylos", NULL};
		int pid = sys_create_process("phylos", args, &phylo, 1, fds);
		return wait(pid, piped, background);
	} else {
		printColor("\nCommand not found. Type \"help\" for command list\n", RED);
	}
	return 0;
}

void infiniteLoop(char ** argv) {
	while (1)
		;
}

void parseCommand(char ** argv, char * buffer, int maxArgs) {
	int i = 0;
	int j = 0;
	while (buffer[i] != 0 && j < maxArgs) {
		while (buffer[i] == ' ') {
			i++;
		}
		if (buffer[i] == 0) {
			break;
		}
		argv[j] = &(buffer[i]);
		j++;
		while (buffer[i] != ' ' && buffer[i] != 0) {
			i++;
		}
		buffer[i] = 0;
		i++;
	}
	argv[j] = NULL;
}