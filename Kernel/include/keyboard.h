#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <sync.h>
#include <keyboard.h>
#include <videodriver.h>

#define BUFFER_SIZE 512
#define MAX_KEY_PRESSED 127

void keyboard_handler();

char getChar();

extern unsigned char getKey();

void initKeyboard();


#endif