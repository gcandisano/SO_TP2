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