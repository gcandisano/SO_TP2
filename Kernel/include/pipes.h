#ifndef PIPES_H
#define PIPES_H

#include <stdint.h>

#define PIPE_BUFFER_SIZE 1024
#define MAX_PIPES 50
#define EOF -1 

typedef struct pipesType {
    int name;
    unsigned int readPos;
    unsigned int writePos;
    uint8_t eof;
    int leftToRead;
    char *pipeBuffer;
    int readSemId;
    int writeSemId;
} pipesType;

void pipesInit();
int namedPipeCreate(int name);
int anonPipeCreate();
void pipeDestroy(int id);
void sendEOF(int id);
int getPipeId(int name);
int readPipe(int id, char *dest, unsigned int count);
int writePipe(int id, const char *src, unsigned int count);
void sendEOFToCurrent();

#endif
