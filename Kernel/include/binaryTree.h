#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stdint.h>

typedef struct node_t {
    struct node *previous;
    struct node *next;
    uint8_t order;
    uint8_t free;
} node_t;

void initList(node_t *node);

void removeNode(node_t *node);

char isEmpty(node_t *node);

void push(node_t *node, node_t *newNode);

node_t *pop(node_t *node);

#endif