// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
// #define BUDDY
#ifndef BUDDY
#include <mm_manager.h>

struct node memory_list;

memoryData memory_data;

void * list_address;

void startMemoryManager(const void * start_address, uint64_t size) {
	list_address = (void *) start_address;
	memory_list.address = (void *) start_address + LIST_SPACE;
	memory_list.size = size;
	memory_list.is_free = true;
	memory_list.prev = NULL;
	memory_list.next = NULL;

	memory_data.total = size;
	memory_data.free = size;
	memory_data.used = 0;
}

void * malloc(uint64_t size) {
	if (size <= 0) {
		return NULL;
	}
	if (list_address >= (void *) 0x600000 + (128 * 1024 * 1024)) {
		printString("No More memory\n");
		return NULL;
	}
	NodePtr current = &memory_list;
	while (current != NULL && !(current->is_free && current->size >= size)) {
		current = current->next;
	}

	if (current == NULL) {
		return NULL;  // There is no more empty space
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

	memory_data.free -= size;
	memory_data.used += size;

	return current->address;
}

int free(void * address) {
	NodePtr current = &memory_list;
	while (current != NULL && current->address != address) {
		current = current->next;
	}

	if (current == NULL) {
		return 1;  // Address not found
	}

	if (current->is_free) {
		return 2;  // Address already free
	}

	current->is_free = true;

	memory_data.free += current->size;
	memory_data.used -= current->size;

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
	return 0;
}

MemoryDataPtr getMemoryData() {
	return &memory_data;
}
#endif