#include <stdint.h>
#include <stdbool.h>

#define NULL ((void *)0)
typedef uint64_t * ptr_t;

struct node {
    ptr_t address;
    uint64_t size;
    bool is_free;
    struct node * prev;
    struct node * next;
};

typedef struct node * node_ptr;

struct node memory_list;

ptr_t list_address;

void init_memory_manager(uint64_t start_address, uint64_t size) {
    list_address = start_address;
    memory_list.address = (ptr_t) start_address + 0x10000;
    memory_list.size = size;
    memory_list.is_free = true;
    memory_list.prev = NULL;
    memory_list.next = NULL;
}

ptr_t malloc(uint64_t size) {
    if (size <= 0) {
        return NULL;
    }
    node_ptr current = &memory_list;
    while (current != NULL && !(current->is_free && current->size >= size)) {
        current = current->next;
    }

    if (current == NULL) {
        return NULL; // There is no more empty space
    }

    node_ptr new_node = (node_ptr) (list_address + sizeof(struct node));
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

void free(ptr_t address) {
    node_ptr current = &memory_list;
    while (current != NULL && current->address != address) {
        current = current->next;
    }

    if (current == NULL) {
        return; // Address not found
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