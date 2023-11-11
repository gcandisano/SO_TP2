// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>

int filter(char ** args) {
	printChar('\n');
	char buff[SIZE] = {0};
	char c;
	int i = 0;
	int j = 0;
	while ((c = getChar()) != -1) {
		if (c > 20 && c < 127) {
			printChar(c);
			if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'I' || c == 'E' ||
			    c == 'O' || c == 'U')
				buff[i++] = c;
		}
		if (c == '\n') {
			printChar('\n');
			while (j < i) {
				printChar(buff[j]);
				j++;
			}
			printChar('\n');
			i = 0;
			j = 0;
		}
		if (c == '\b') {
			if (i != 0) {
				printChar('\b');
				i--;
			}
		}
	}
	return 0;
}