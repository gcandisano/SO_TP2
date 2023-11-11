// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>

#define SIZE 1024

int cat(char ** args) {
	printChar('\n');
	char buff[SIZE] = {0};
	char c;
	int i = 0;
	int j = 0;
	while ((c = getChar()) != -1) {
		if (c == '\b') {
			if (i != 0) {
				printChar('\b');
				i--;
			}
		} else if (c == '\n') {
			printChar('\n');
			while (j < i) {
				printChar(buff[j]);
				j++;
			}
			i = 0;
			j = 0;
			printChar('\n');
		} else if (c > 20 && c < 127) {
			if (i != SIZE) {
				buff[i] = c;
				printChar(buff[i]);
				i++;
			}
		}
	}

	return 0;
}