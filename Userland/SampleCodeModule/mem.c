#include <commands.h>

void memInfo() {
	MemoryDataPtr memData = sys_mem_data();
	printfColor("\n\nMemory info:\n", YELLOW);
	printfColor("\tTotal memory: %d bytes\n", CYAN, memData->total);
	printfColor("\tUsed memory: %d bytes\n", CYAN, memData->used);
	printfColor("\tFree memory: %d bytes\n", CYAN, memData->free);
	return;
}