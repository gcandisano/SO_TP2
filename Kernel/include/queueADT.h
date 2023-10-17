#include <mm_manager.h>

typedef struct QueueCDT * QueueADT;

QueueADT createQueue();

void enqueue(QueueADT queue, void * data);

void * dequeueByData(QueueADT queue, void * data);

void * findElement(Queue *queue, uint8_t (*condition)(void *));