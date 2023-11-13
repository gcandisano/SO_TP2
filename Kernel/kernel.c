// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <colors.h>
#include <homero.h>
#include <idtLoader.h>
#include <keyboard.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <pipes.h>
#include <scheduler.h>
#include <sound.h>
#include <stdint.h>
#include <sync.h>
#include <videodriver.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

extern void saveOriginalRegs();
extern void _hlt();

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void *) 0x400000;
static void * const sampleDataModuleAddress = (void *) 0x500000;

static void * const memoryManagerStart = (void *) 0x600000;

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase() {
	return (void *) ((uint64_t) &endOfKernel + PageSize * 8  // The size of the stack itself, 32KiB
	                 - sizeof(uint64_t)                      // Begin at the top of the stack
	);
}

void * initializeKernelBinary() {
	void * moduleAddresses[] = {sampleCodeModuleAddress, sampleDataModuleAddress};

	loadModules(&endOfKernelBinary, moduleAddresses);
	clearBSS(&bss, &endOfKernel - &bss);
	return getStackBase();
}

void idle() {
	while (1) {
		_hlt();
	}
}

char * shellArgs[] = {"shell", NULL};
char * idleArgs[] = {"idle", NULL};
int defaultFds[] = {0, 1};

int main() {
	loadIdt();  // Setup idt before terminal runs

	drawImage(homero, 100, 100);
	printStringColor("Press any key to start. If not found, press CTRRRRL \n\n", YELLOW);
	// playSimpsons();

	saveOriginalRegs();

	startMemoryManager(memoryManagerStart, (1 << 27));

	semInit();

	initPipes();

	initKeyboard();

	startScheduler();

	int shell = createProcess("shell", 0, 8192, shellArgs, sampleCodeModuleAddress, 1, defaultFds);

	int idlePid = createProcess("idle", 0, 1024, idleArgs, &idle, 0, defaultFds);

	changePriority(idlePid, 0);

	startShell(shell);

	// ((EntryPoint)sampleCodeModuleAddress)(); //Calling sampleCodeModule's main address
	beep();
	return 0;
}