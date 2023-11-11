// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <pipes.h>
#include <sync.h>

pipesType pipesArr[MAX_PIPES];
int namePipeMax = 1;
int receivedEOF = 0;

void initPipes() {
	pipesArr[0].name = 0;
	for (int i = 1; i < MAX_PIPES; i++) {
		pipesArr[i].name = -1;
	}
}

int createNamedPipe(int name) {
	int availableSpace = 0;
	for (int i = 2; i < MAX_PIPES && availableSpace == 0; i++) {
		if (pipesArr[i].name == -1) {
			availableSpace = i;
		} else if (pipesArr[i].name == name) {
			return -1;
		}
	}
	pipesArr[availableSpace].name = name;
	pipesArr[availableSpace].readPos = 0;
	pipesArr[availableSpace].writePos = 0;
	pipesArr[availableSpace].eof = 0;
	pipesArr[availableSpace].leftToRead = 0;
	pipesArr[availableSpace].pipeBuffer = (char *) malloc(PIPE_BUFFER_SIZE);
	pipesArr[availableSpace].readSemId = semCreateAnon(0);
	pipesArr[availableSpace].writeSemId = semCreateAnon(PIPE_BUFFER_SIZE);

	if (name >= namePipeMax) {
		namePipeMax = name + 1;
	}
	return availableSpace;
}

int createAnonPipe() {
	return createNamedPipe(namePipeMax);
}

void destroyPipe(int id) {
	pipesArr[id].name = -1;
	pipesArr[id].readPos = 0;
	pipesArr[id].writePos = 0;
	pipesArr[id].eof = 0;
	semDestroy(pipesArr[id].readSemId);
	semDestroy(pipesArr[id].writeSemId);
	free(pipesArr[id].pipeBuffer);
}

void sendEOF(int id) {
	pipesArr[id].eof = 1;
}

int getPipeId(int name) {
	for (int i = 0; i < MAX_PIPES; i++) {
		if (pipesArr[i].name == name) {
			return i;
		}
	}
	return -1;
}

int readPipe(int id, char * dest, unsigned int count) {
	if (pipesArr[id].leftToRead == 0 && pipesArr[id].eof) {
		dest[0] = EOF;
		return -1;
	}

	for (int i = 0; i < count; i++) {
		semWait(pipesArr[id].readSemId);

		if (pipesArr[id].readPos == PIPE_BUFFER_SIZE) {
			pipesArr[id].readPos = 0;
		}

		dest[i] = pipesArr[id].pipeBuffer[pipesArr[id].readPos++];

		if (dest[i] == -1) {
			pipesArr[id].leftToRead--;
			semPost(pipesArr[id].writeSemId);
			return -1;
		}
		pipesArr[id].leftToRead--;
		semPost(pipesArr[id].writeSemId);
	}
	return 0;
}

int writePipe(int id, const char * src, unsigned int count) {
	if (pipesArr[id].eof && receivedEOF) {
		return -1;
	}
	for (int i = 0; i < count; i++) {
		semWait(pipesArr[id].writeSemId);
		if (pipesArr[id].writePos == PIPE_BUFFER_SIZE) {
			pipesArr[id].writePos = 0;
		}
		if (pipesArr[id].eof && !receivedEOF) {
			pipesArr[id].pipeBuffer[pipesArr[id].writePos++] = EOF;
			pipesArr[id].leftToRead++;
			receivedEOF = 1;
			semPost(pipesArr[id].readSemId);
		}
		pipesArr[id].pipeBuffer[pipesArr[id].writePos++] = (char) src[i];
		pipesArr[id].leftToRead++;
		semPost(pipesArr[id].readSemId);
	}
	return 1;
}

void sendEOFToCurrentProcess() {
	PCB * pcb = findPcb(getForegroundProcess());
	if (pcb->fd[READ_FD] != -1) {
		sendEOF(pcb->fd[READ_FD]);
	}
}