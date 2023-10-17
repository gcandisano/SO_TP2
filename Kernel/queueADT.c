#include <mm_manager.h>
#include <queueADT.h>
#include <process.h>

typedef struct QueueCDT * QueueADT;

typedef struct Node {
    PCB * data;
    struct Node * next;
} Node;

typedef struct QueueCDT {
    Node * first;
    Node * last;
} QueueCDT;


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

    void * dataToReturn = current->data;
    free(current);
    return dataToReturn;
}

void * findElement(QueueADT queue, uint8_t (*condition)(void *)) {
    Node *current = queue->first;
    while (current != NULL) {
        if (condition(current->data)) {
            return current->data;
        }
        current = current->next;
    }

    return NULL;
}

void * findElementByPid(QueueADT queue, int pid) {
    Node *current = queue->first;
    while (current != NULL) {
        if (current->data->pid == pid) {
            return current->data;
        }
        current = current->next;
    }

    return NULL;
}