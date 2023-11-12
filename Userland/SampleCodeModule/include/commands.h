#ifndef COMMANDS_H
#define COMMANDS_H

#include <colors.h>
#include <structs.h>
#include <uStrings.h>
#include <userio.h>
#include <usyscalls.h>

extern void fillRegs();

#define READY   0
#define BLOCKED 1
#define RUNNING 2
#define ZOMBIE  3
#define DEAD    4
#define EOF     -1
#define SIZE    1024

int processesInfo();
int killProcess(char ** argv);
int blockProcess(char ** argv);
int unblockProcess(char ** argv);
int changePriority(char ** argv);
void yield();
char * getTime();
char * getDate();
void timePrint();
void datePrint();
void printRegs();
void fillRegisters();
int cat(char ** args);
int wc(char ** args);
int filter(char ** args);
int phylo(char ** args);

#endif