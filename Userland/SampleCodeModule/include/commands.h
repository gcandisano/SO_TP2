#ifndef COMMANDS_H
#define COMMANDS_H

#include <structs.h>
#include <usyscalls.h>
#include <colors.h>
#include <userio.h>
#include <uStrings.h>

extern void fillRegs();

#define READY   0
#define BLOCKED 1
#define RUNNING 2
#define ZOMBIE  3
#define DEAD    4

int processesInfo();
int killProcess(char ** argv);
int blockProcess(char ** argv);
int unblockProcess(char ** argv);
int changePriority(char ** argv);
void yield();
char * getTime();
char * getDate();
void printRegs();
void fillRegisters();
int cat(char **args);

#endif