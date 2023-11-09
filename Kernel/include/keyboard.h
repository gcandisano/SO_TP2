#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <keyboard.h>
#include <pipes.h>
#include <sync.h>
#include <videodriver.h>

#define BUFFER_SIZE     512
#define MAX_KEY_PRESSED 127
#define LEFT_CTRL       0x1D
#define RELEASE_SHIFT   0xAA
#define CAPS_LOCK       0x3A

void keyboard_handler();

char getChar();

char getCharNoBlock();

extern unsigned char getKey();

void initKeyboard();

#endif