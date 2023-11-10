#ifndef COMMANDS_H
#define COMMANDS_H

#include <userio.h>
#include <uStrings.h>
#include <usyscalls.h>
#include <colors.h>

#define READY   0
#define BLOCKED 1
#define RUNNING 2
#define ZOMBIE  3
#define DEAD    4


char * getTime();
char * getDate();
void memInfo();
int processesInfo();
int killProcess(char ** argv);
int nice(char ** argv);
int block(char ** argv);
void printRegs();
void fillRegisters();



#endif