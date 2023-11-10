#ifndef SHELL_H
#define SHELL_H

#include <stddef.h>
#include <structs.h>
#include <uStrings.h>

extern void divideByZero();
extern void invalidOpcode();

void shell();

int commandMatch(char * str1, char * command, int count);

void analizeBuffer(char * buffer, int count);

void memInfo();

void infiniteLoop();


#endif