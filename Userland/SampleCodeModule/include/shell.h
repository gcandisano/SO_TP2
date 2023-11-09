#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>
#include <structs.h>

#define READY 0
#define BLOCKED 1
#define RUNNING 2
#define ZOMBIE 3
#define DEAD 4

extern void divideByZero();
extern void invalidOpcode();

void shell();

int commandMatch(char * str1, char * command, int count);

void analizeBuffer(char * buffer, int count);

void memInfo();

void processesInfo();

void infiniteLoop();

#endif