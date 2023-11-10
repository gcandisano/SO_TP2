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
			printf("\n");
			while (j != i) {
				printf("%c", buff[j]);
				j++;
			}
			i = 0;
			j = 0;
			printf("\n");
		} else {
			if (i != SIZE) {
				buff[i] = c;
				printf("%c", buff[i]);
				i++;
			}
		}
	}

	return 0;
}