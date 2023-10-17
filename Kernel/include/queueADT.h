#ifndef QUEUE_ADT_H
#define QUEUE_ADT_H

#include <mm_manager.h>

typedef struct QueueCDT * QueueADT;

QueueADT createQueue();

void enqueue(QueueADT queue, void * data);

void * dequeueByData(QueueADT queue, void * data);

void * findElement(QueueADT queue, uint8_t (*condition)(void *));

#endif