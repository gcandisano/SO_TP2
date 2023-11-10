#ifndef QUEUE_ADT_H
#define QUEUE_ADT_H

#include <mm_manager.h>

typedef struct QueueCDT * QueueADT;

QueueADT createQueue();

void enqueue(QueueADT queue, void * data);

void * dequeue(QueueADT queue);

void * dequeueByData(QueueADT queue, void * data);

int isEmpty(QueueADT queue);

void * peek(QueueADT queue);

void * findElement(QueueADT queue, uint8_t (*condition)(void *));

void ** findElements(QueueADT queue, uint8_t (*condition)(void *, int), int pid);

void ** getAllElements(QueueADT queue);

void deleteQueue(QueueADT queue);

#endif