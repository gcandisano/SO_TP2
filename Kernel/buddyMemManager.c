// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#ifdef BUDDY

#include <mm_manager.h>
#include <stdint.h>

#define MIN_EXP 5  // Block Size
#define LEVELS  32

typedef struct Block {
	uint8_t exp;
	bool is_used;
	struct Block * prev;
	struct Block * next;
} Block;

void * firstAddress;
uint8_t maxExp;
Block * blocks[LEVELS];

memoryData memory_data;

Block * createBlock(void * ptrToAllocate, uint8_t exp, Block * next);

Block * removeBlock(Block * block);

Block * merge(Block * block, Block * buddy);

static void split(uint8_t idx);

unsigned int log(uint64_t n, int base) {
	unsigned int count = 0;
	while (n /= base)
		count++;
	return count;
}

void startMemoryManager(const void * start_address, uint64_t size) {
	printString("Using Buddy Memory Manager\n\n");
	firstAddress = (void *) start_address;
	maxExp = log(size, 2);

	if (MIN_EXP > maxExp)
		return;

	for (uint8_t i = 0; i < LEVELS; i++)
		blocks[i] = NULL;

	memory_data.free = size;
	memory_data.total = size;
	memory_data.used = 0;

	blocks[maxExp - 1] = createBlock(firstAddress, maxExp, NULL);
}

void * malloc(uint64_t size) {
	uint8_t idToAlloc = log(size + sizeof(Block), 2);

	idToAlloc = idToAlloc < MIN_EXP - 1 ? MIN_EXP - 1 : idToAlloc;

	if (idToAlloc >= maxExp)
		return NULL;

	if (blocks[idToAlloc] == NULL) {
		uint8_t closestId = 0;
		for (uint8_t i = idToAlloc + 1; i < maxExp && !closestId; i++)
			if (blocks[i] != NULL)
				closestId = i;
		if (closestId == 0)
			return NULL;

		while (closestId > idToAlloc)
			split(closestId--);
	}
	Block * block = blocks[idToAlloc];
	removeBlock(block);
	block->is_used = true;
	block->prev = NULL;
	block->next = NULL;

	uint64_t blockSize = 1L << block->exp;
	memory_data.used += blockSize;
	memory_data.free -= blockSize;

	return (void *) block + sizeof(Block);
}

static void split(uint8_t id) {
	Block * block = blocks[id];
	removeBlock(block);
	Block * buddyBlock = (Block *) ((void *) block + (1L << id));
	createBlock((void *) buddyBlock, id, blocks[id - 1]);
	blocks[id - 1] = createBlock((void *) block, id, buddyBlock);
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
	Block * buddyBlock = (Block *) ((uint64_t) firstAddress + (((uint64_t) relativePosition) ^ (1L << block->exp)));
	while (!buddyBlock->is_used && buddyBlock->exp == block->exp && block->exp < maxExp) {
		block = merge(block, buddyBlock);
		relativePosition = (uint64_t) ((void *) block - firstAddress);
		buddyBlock = (Block *) ((uint64_t) firstAddress + (((uint64_t) relativePosition) ^ (1L << block->exp)));
	}
	blocks[block->exp - 1] = createBlock((void *) block, block->exp, blocks[block->exp - 1]);
	return 0;
}

Block * merge(Block * block, Block * buddy) {
	removeBlock(buddy);
	Block * leftBlock = block < buddy ? block : buddy;
	leftBlock->exp++;
	return leftBlock;
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

MemoryDataPtr getMemoryData() {
	return &memory_data;
}
#endif