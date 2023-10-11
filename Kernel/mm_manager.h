#include <stdint.h>

typedef uint64_t * ptr_t;

void init_memory_manager(uint64_t start_address, uint64_t size);

ptr_t malloc(uint64_t size);

void free(ptr_t address);