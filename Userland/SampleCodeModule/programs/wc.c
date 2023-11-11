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