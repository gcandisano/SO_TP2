// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>

int wc(char ** args) {
	printChar('\n');
	int c, counter = 1;
	while ((c = getChar()) != EOF) {
		if (c == '\n') {
			printChar('\n');
			counter++;
		} else if (c > 20 && c < 127) {
			printChar(c);
		}
	}
	printf("\nTotal lines: %d\n", counter);
	return 0;
}