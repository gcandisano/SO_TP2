#include <queueADT.h>

typedef struct QueueCDT * QueueADT;

typedef struct Node {
    void * data;
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

void * dequeue(QueueADT queue) {
    if (isEmpty(queue)) {
        return NULL;
    }
    Node * aux = queue->first;
    void * data = aux->data;
    queue->first = queue->first->next;

    if (queue->first == NULL) {
        queue->last = NULL;
    }
    free(aux);
    return data;
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

int isEmpty(QueueADT queue) {
    return queue->first == NULL;
}

void * peek(QueueADT queue) {
    if (queue->first == NULL) {
        return NULL;
    }
    return queue->first->data;
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

void ** findElements(QueueADT queue, uint8_t (*condition)(void *, int), int pid) {
    void ** elements = malloc(sizeof(void *) * 100);
    if (elements == NULL) {
        return NULL;
    }
    int dim = 0;
    Node *current = queue->first;
    while (current != NULL) {
        if (condition(current->data, pid)) {
            elements[dim++] = current->data;
        }
        current = current->next;
    }
    elements[dim] = NULL;
    return elements;
}

void deleteQueue(QueueADT queue) {
    Node * current = queue->first;
    Node * next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    free(queue);
}