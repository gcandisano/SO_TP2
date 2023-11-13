// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef BUDDY

#include <lib.h>
#include <mm_manager.h>
#include <stdint.h>

#define MIN_EXP    5
#define MAX_BLOCKS 32

typedef struct Block {
	uint8_t exp;
	bool is_used;
	struct Block * prev;
	struct Block * next;
} Block;

void * firstAddress;
uint8_t maxExp;
Block * blocks[MAX_BLOCKS];

memoryData memory_data;

Block * createBlock(void * address, uint8_t exp, Block * next);

Block * removeBlock(Block * block);

Block * join(Block * block, Block * buddy);

void divide(uint8_t index);

void startMemoryManager(const void * start_address, uint64_t size) {
	printString("Using Buddy Memory Manager\n\n");
	firstAddress = (void *) start_address;
	maxExp = log(size, 2);

	if (MIN_EXP > maxExp)
		return;

	memset(blocks, 0, sizeof(Block *) * MAX_BLOCKS);

	memory_data.free = size;
	memory_data.total = size;
	memory_data.used = 0;

	blocks[maxExp - 1] = createBlock(firstAddress, maxExp, NULL);
}

void * malloc(uint64_t size) {
	uint8_t newBlockIndex = log(size + sizeof(Block), 2);

	newBlockIndex = (newBlockIndex < MIN_EXP - 1) ? (MIN_EXP - 1) : newBlockIndex;

	if (newBlockIndex >= maxExp)
		return NULL;

	if (blocks[newBlockIndex] == NULL) {
		uint8_t closestId = 0;
		for (uint8_t i = newBlockIndex + 1; i < maxExp && !closestId; i++) {
			if (blocks[i] != NULL)
				closestId = i;
		}
		if (closestId == 0)
			return NULL;

		while (closestId > newBlockIndex)
			divide(closestId--);
	}
	Block * block = blocks[newBlockIndex];
	removeBlock(block);
	block->prev = NULL;
	block->next = NULL;
	block->is_used = true;

	uint64_t blockSize = 1L << block->exp;
	memory_data.used += blockSize;
	memory_data.free -= blockSize;

	return (void *) block + sizeof(Block);
}

int free(void * address) {
	Block * block = (Block *) (address - sizeof(Block));
	if (!block->is_used)
		return -1;
	block->is_used = false;

	uint64_t blockSize = 1L << block->exp;
	memory_data.free += blockSize;
	memory_data.used -= blockSize;

	uint64_t relativePosition = (uint64_t) ((void *) block - firstAddress);
	Block * buddy = (Block *) ((uint64_t) firstAddress + ((relativePosition) ^ (1L << block->exp)));
	while (!buddy->is_used && buddy->exp == block->exp && block->exp < maxExp) {
		block = join(block, buddy);
		relativePosition = (uint64_t) ((void *) block - firstAddress);
		buddy = (Block *) ((uint64_t) firstAddress + (relativePosition ^ (1L << block->exp)));
	}
	blocks[block->exp - 1] = createBlock(block, block->exp, blocks[block->exp - 1]);
	return 0;
}

Block * join(Block * block, Block * buddy) {
	removeBlock(buddy);
	Block * leftBlock = block < buddy ? block : buddy;
	leftBlock->exp++;
	return leftBlock;
}

void divide(uint8_t index) {
	Block * block = blocks[index];
	removeBlock(block);
	Block * buddy = (Block *) ((void *) block + (1L << index));
	createBlock(buddy, index, blocks[index - 1]);
	blocks[index - 1] = createBlock(block, index, buddy);
}

Block * createBlock(void * address, uint8_t exp, Block * next) {
	Block * block = (Block *) address;
	block->exp = exp;
	block->is_used = false;
	block->prev = NULL;
	block->next = next;
	if (next != NULL) {
		next->prev = block;
	}
	return block;
}

Block * removeBlock(Block * block) {
	if (block->prev != NULL)
		block->prev->next = block->next;
	else
		blocks[block->exp - 1] = block->next;

	if (block->next != NULL)
		block->next->prev = block->prev;
	return block->next;
}

MemoryDataPtr getMemoryData() {
	return &memory_data;
}
#endif