#include <mm_manager.h>
#include <queueADT.h>

typedef struct QueueCDT * QueueADT;

typedef struct QueueCDT {
    Node * first;
    Node * last;
} QueueCDT;

typedef struct Node {
    void * data;
    Node * next;
} Node;

QueueADT createQueue() {
    QueueADT queue = malloc(sizeof(QueueCDT));
    queue->first = NULL;
    queue->last = NULL;
    return queue;
}

void enqueue(QueueADT queue, void * data) {
    Node * node = malloc(sizeof(Node));
    node->data = data;
    node->next = NULL;
    if (queue->first == NULL) {
        queue->first = node;
    } else {
        queue->last->next = node;
    }
    queue->last = node;
}

void * dequeueByData(QueueADT queue, void * data) {
    Node * current = queue->first;
    Node * previous = NULL;
    while (current != NULL && current->data != data) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        return NULL;
    }

    if (previous == NULL) {
        queue->first = current->next;
    } else {
        previous->next = current->next;
    }

    if (current == queue->last) {
        queue->last = previous;
    }

    void * data = current->data;
    free(current);
    return data;
}