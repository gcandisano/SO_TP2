// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/* sampleCodeModule.c */
#include <userio.h>
#include <usyscalls.h>
#include <colors.h>
#include <shell.h>
#include <pong.h>

int main() {
	// Choose between terminal or pong
	printColor("Welcome to HomerOS. Choose between terminal or pong (t/p): \n\n", GREEN);

	unsigned char c;
	do {
		c = getChar();
	} while (c != 't' && c != 'p');

	// getChar();

	if (c == 'p') {
		char * args[2] = {"pong", NULL};
		int fds[3] = {0, 1, 2}; 
		int pid = sys_create_process("pong", args, &pong, 1, fds);
		sys_wait_pid(pid);
	}

	sys_clear_screen();

	shell();

	return 0;
}