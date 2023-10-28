// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <mm_manager.h>

struct node {
    void * address;
    uint64_t size;
    bool is_free;
    struct node * prev;
    struct node * next;
};

typedef struct node * NodePtr;

struct node memory_list;

void * list_address;

void startMemoryManager(const void * start_address, uint64_t size) {
    list_address = (void *) start_address;
    memory_list.address = (void *) start_address + 0x100000;
    memory_list.size = size;
    memory_list.is_free = true;
    memory_list.prev = NULL;
    memory_list.next = NULL;
}

void * malloc(uint64_t size) {
    if (size <= 0) {
        return NULL;
    }
    NodePtr current = &memory_list;
    while (current != NULL && !(current->is_free && current->size >= size)) {
        current = current->next;
    }

    if (current == NULL) {
        return NULL; // There is no more empty space
    }

    NodePtr new_node = (NodePtr) (list_address + sizeof(struct node));
    list_address += sizeof(struct node);
    new_node->address = current->address + size;
    new_node->size = current->size - size;
    new_node->is_free = true;
    new_node->prev = current;
    new_node->next = current->next;

    current->size = size;
    current->is_free = false;
    current->next = new_node;

    return current->address;
}

void free(void * address) {
    NodePtr current = &memory_list;
    while (current != NULL && current->address != address) {
        current = current->next;
    }

    if (current == NULL) {
        return; // Address not found
    }

    if (current->is_free) {
        return; // Address already free
    }

    current->is_free = true;

    if (current->prev != NULL && current->prev->is_free) {
        current->prev->size += current->size;
        current->prev->next = current->next;
        current->next->prev = current->prev;
        current = current->prev;
    }

    if (current->next != NULL && current->next->is_free) {
        current->size += current->next->size;
        current->next = current->next->next;
        current->next->prev = current;
    }
}