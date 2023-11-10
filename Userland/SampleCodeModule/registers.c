#include <commands.h>

extern void fillRegs();

void printRegs() {
	uint64_t regs[17];
	char * regsnames[] = {"RAX ",
	                      "RBX ",
	                      "RCX ",
	                      "RDX ",
	                      "RSI ",
	                      "RDI ",
	                      "RBP ",
	                      "RSP ",
	                      "R8  ",
	                      "R9  ",
	                      "R10 ",
	                      "R11 ",
	                      "R12 ",
	                      "R13 ",
	                      "R14 ",
	                      "R15 ",
	                      "RIP "};
	print("\n\n");
	sys_get_regs(regs);
	for (int i = 0; i < 17; i++) {
		char hex[17];
		intToHex(regs[i], hex);
		printf("%s 0x%s\n", regsnames[i], hex);
	}
}

void fillRegisters() {
	printColor("\n\nFilling registers...\n", YELLOW);
	printColor("Press CTRL to save them.\n", CYAN);
	fillRegs();
}
