#include <string.h>
#include <keyboard.h>
#include <videodriver.h>

#define BUFFER_SIZE 512

static char buffer[BUFFER_SIZE] = {0};
static int elemCount = 0;
static int readIndex = 0;
static int writeIndex = 0;

static char shiftPressed = 0;
static char capsLocked = 0;

static const char charHexMap[256] = {       
        0,    0,  '1',  '2',  '3',  '4',  '5',  '6',   '7',  '8',  '9',   '0',   '\'',  '<', '\b',
    '\t', 'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',   'o',  'p',  '\\',   '+', '\n',   
        0,     'a',  's', 'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '{',
    '|',  1/*shift*/,  '}',  'z',  'x', 'c', 'v',  'b',  'n',  'm',  ',',  '.',  '-',    0,  
    '*',     0,  ' ',    0,     0,     0/*60*/,    0,       0,         0, 
    0,     0,     0,    0,      0,      0,      0,      0,         17/*up*/,
    0,      0,      18/*left*/,    0,     19/*right*/,     0,      0,      20/*down*/,      0,
};
static const char charCapsHexMap[256] = {       
        0,    0,  '!',  '\"',  '#',  '$',  '%',  '&',   '/',  '(',  ')',   '=',   '?',  '>', '\b',
    '\t', 'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',   'O',  'P',  '^',   '*', '\n',   
        0,     'A',  'S', 'D',  'F',  'G',  'H',  'J',  'K',  'L',  '~',  '[',
    '/',  1/*shift*/,  ']',  'Z',  'X', 'C', 'V',  'B',  'N',  'M',  ';',  ':',  '_',    0,  
    '*',     0,  ' ',    0,     0,     0/*60*/,    0,       0,         0, 
    0,     0,     0,    0,      0,      0,      0,      0,         17/*up*/,
    0,      0,      18/*left*/,    0,     19/*right*/,     0,      0,      20/*down*/,      0,
};

char isLetter(unsigned char key) {
    char c = charHexMap[key];
    return (c >= 'a' && c <= 'z');
}

void keyboard_handler() {
    unsigned char key = getKey();
    if (key < 83 || key == 0xAA/* Release SHIFT */ || key == 0x3A /* CAPS Lock */) { // 83 elems in the charHexMap
        if (elemCount >= BUFFER_SIZE) return;  // buffer is full
        
        // make the array circular
        if (writeIndex >= BUFFER_SIZE)
            writeIndex = 0;

        if (charHexMap[key] == 1 && !shiftPressed) {
            shiftPressed = 1;
            return;
        }
        if (key == 0xAA) {
            shiftPressed = 0;
            return;
        }
        if (key == 0x3a) {
            capsLocked = !capsLocked;
            return;
        }

        buffer[writeIndex] = !isLetter(key) ? (shiftPressed ? charCapsHexMap[key] : charHexMap[key]): ((shiftPressed && !capsLocked) || (!shiftPressed && capsLocked)) ? charCapsHexMap[key] : charHexMap[key];

        // update iterators
        elemCount++;
        writeIndex++;
    }
}

char getChar(){
    if (elemCount == 0) { 
        return 0xFF; // buffer is empty
    }

    char toReturn = buffer[readIndex];
    
    // update iterators
    elemCount--;
    readIndex++;

    // make the array circular
    if (readIndex == BUFFER_SIZE) readIndex = 0;
    
    return toReturn;
}