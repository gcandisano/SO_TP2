// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <commands.h>

char * getTime() {
	static char bufferTime[9];
	sys_get_time(bufferTime);
	return bufferTime;
}

char * getDate() {
	static char bufferDate[9];
	sys_get_date(bufferDate);
	return bufferDate;
}

void timePrint() {
	printfColor("\n\nTime of OS: ", YELLOW);
	printfColor("%s\n", CYAN, getTime());
}

void datePrint() {
	printfColor("\n\nDate of OS: ", YELLOW);
	printfColor("%s\n", CYAN, getDate());
}