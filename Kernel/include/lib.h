#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char * cpuVendor(char * result);

/* Used for sound */
void outb(int memoryMap, int output);
int inb(int memoryMap);
uint64_t * createStack(uint64_t * stackStart, void * code, char ** args, void * wrapper);
void forceScheduler();

unsigned int strlen(char * str);
void strcpy(char * dest, char * src);
int strcmp(char * str1, char * str2);

#endif