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

static char * commandsNames[] = {"help",  "time", "date",   "registers", "fillregs", "div0",    "invalidop", "pong",
                                 "clear", "mem",  "ps",     "kill",      "block",    "unblock", "yield",     "nice",
                                 "loop",  "cat",  "testmm", "testpro",   "testprio", "testsync"};

static char * commands[] = {
    "\thelp: gives you a list of all existent commands.\n",
    "\ttime: prints the time of the OS.\n",
    "\tdate: prints the date of the OS.\n",
    "\tregisters: print the state of the registers at the time you screenshot them with CTRL key.\n",
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
    "\ttestsync: test sync.\n"};

char * loopArgs[2] = {"loop", NULL};
char * testmmArgs[3];
char * testProArgs[3];
char * testSyncArgs[4];

int fds[3] = {0, 1, 0};

void shell() {
	printColor("Welcome to HomerOS. Type \"help\" for command list\n", ORANGE);
	printColor("\nHomerOS: $> ", GREEN);

	int count = 0;
	char buffer[1024] = {0};
	char oldBuffer[1024] = {0};
	char flag = 0;  // Used for up arrow
	while (1) {
		unsigned char c = getChar();
		if (c == '\n') {
			buffer[count] = 0;
			analizeBuffer(buffer, count);
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

int commandMatch(char * str1, char * command, int count) {
	int i = 0;
	while (i < count && str1[i] != 0 && command[i] != 0 && str1[i] == command[i]) {
		i++;
		if (str1[i] == ' ') {
			i--;
			break;
		}
	}
	return str1[i] == command[i];
}

void analizeBuffer(char * buffer, int count) {
	if (count <= 0)
		return;
	if (commandMatch(buffer, "help", count) || commandMatch(buffer, "HELP", count)) {
		printColor("\n\nComandos disponibles:\n\n", YELLOW);
		for (int i = 0; i < COMMANDS_QUANTITY; i++) {
			printColor(commands[i], CYAN);
		}
	} else if (commandMatch(buffer, "time", count)) {
		printfColor("\n\nTime of OS: ", YELLOW);
		printfColor("%s\n", CYAN, getTime());
	} else if (commandMatch(buffer, "date", count)) {
		printfColor("\n\nDate of OS: ", YELLOW);
		printfColor("%s\n", CYAN, getDate());
	} else if (commandMatch(buffer, "registers", count)) {
		printRegs();
	} else if (commandMatch(buffer, "fillregs", count)) {
		fillRegisters();
	} else if (commandMatch(buffer, "clear", count)) {
		sys_clear_screen();
	} else if (commandMatch(buffer, "pong", count)) {
		char * args[2] = {"pong", NULL};
		int fds[3] = {0, 1, 2};
		int pid = sys_create_process("pong", args, &pong, 1, fds);
		sys_wait_pid(pid);
	} else if (commandMatch(buffer, "div0", count)) {
		divideByZero();
	} else if (commandMatch(buffer, "invalidop", count)) {
		invalidOpcode();
	} else if (commandMatch(buffer, "mem", count)) {
		char * args[2] = {"mem", NULL};
		int pid = sys_create_process("mem", args, &memInfo, 1, fds);
		sys_wait_pid(pid);
	} else if (commandMatch(buffer, "ps", count)) {
		char * args[2] = {"ps", NULL};
		int pid = sys_create_process("ps", args, &processesInfo, 1, fds);
		sys_wait_pid(pid);
	} else if (commandMatch(buffer, "kill", count)) {
		char * args[3];
		parseCommand(args, buffer, 3);
		int pid = sys_create_process("kill", args, &killProcess, 1, fds);
		sys_wait_pid(pid);
	} else if (commandMatch(buffer, "block", count)) {
		char * args[3];
		parseCommand(args, buffer, 3);
		int pid = sys_create_process("block", args, &blockProcess, 1, fds);
		sys_wait_pid(pid);
	} else if (commandMatch(buffer, "unblock", count)) {
		char * args[3];
		parseCommand(args, buffer, 3);
		int pid = sys_create_process("unblock", args, &unblockProcess, 1, fds);
		sys_wait_pid(pid);
	} else if (commandMatch(buffer, "yield", count)) {
		char * args[3] = {"yield", NULL};
		int pid = sys_create_process("yield", args, &yield, 1, fds);
		sys_wait_pid(pid);
	} else if (commandMatch(buffer, "nice", count)) {
		char * args[4];
		parseCommand(args, buffer, 4);
		int pid = sys_create_process("nice", args, &changePriority, 1, fds);
		sys_wait_pid(pid);
	} else if (commandMatch(buffer, "loop", count)) {
		sys_create_process("loop", loopArgs, &infiniteLoop, 0, fds);
	} else if (commandMatch(buffer, "testmm", count)) {
		parseCommand(testmmArgs, buffer, 3);
		sys_create_process("testmm", testmmArgs, &test_mm, 1, fds);
	} else if (commandMatch(buffer, "testpro", count)) {
		parseCommand(testProArgs, buffer, 3);
		sys_create_process("testpro", testProArgs, &test_processes, 0, fds);
	} else if (commandMatch(buffer, "testprio", count)) {
		int pid = sys_create_process("testprio", NULL, &test_prio, 1, fds);
		sys_wait_pid(pid);
	} else if (commandMatch(buffer, "testsync", count)) {
		printChar('\n');
		parseCommand(testSyncArgs, buffer, 3);
		int pid = sys_create_process("testsync", testSyncArgs, &test_sync, 0, fds);
		sys_wait_pid(pid);
	} else if (commandMatch(buffer, "boca", count)) {
		sys_clear_screen();
		sys_draw_image(diego, 100, 100);
		playBSong();
		sys_clear_screen();
	} else if (commandMatch(buffer, "cat", count)) {
		char * args[2] = {"cat", NULL};
		int pid = sys_create_process("cat", args, &cat, 0, fds);
		sys_wait_pid(pid);
	} else {
		printColor("\nCommand not found. Type \"help\" for command list\n", RED);
	}
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