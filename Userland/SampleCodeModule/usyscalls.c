// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <usyscalls.h>

extern uint64_t sys_call(uint64_t, uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

uint64_t sys_read(unsigned int fd, char * buffer, unsigned int size) {
	return sys_call((uint64_t) 0, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_read_no_block(unsigned int fd, char * buffer, unsigned int size) {
	return sys_call((uint64_t) 0, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 1, (uint64_t) 0);
}

uint64_t sys_write(unsigned int fd, const char * buffer, unsigned int size) {
	return sys_call((uint64_t) 1, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_write_color(unsigned int fd, const char * buffer, unsigned int size, uint64_t color) {
	/* uint64_t c = color.r;
	c <<= 2;
	c += color.g;
	c <<= 2;
	c += color.b; */
	return sys_call((uint64_t) 2, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, color, (uint64_t) 0);
}

uint64_t sys_get_regs(uint64_t * regsBuff) {
	return sys_call((uint64_t) 3, (uint64_t) regsBuff, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_get_time(char * buffer) {
	return sys_call((uint64_t) 4, (uint64_t) buffer, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_get_date(char * buffer) {
	return sys_call((uint64_t) 5, (uint64_t) buffer, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_clear_screen() {
	return sys_call((uint64_t) 6, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_draw_rectangle(int x, int y, int width, int height, int color) {
	return sys_call((uint64_t) 7, (uint64_t) x, (uint64_t) y, (uint64_t) width, (uint64_t) height, (uint64_t) color);
}

uint64_t sys_play_sound(int freq, int duration, int waitAfter) {
	return sys_call(
	    (uint64_t) 8, (uint64_t) freq, (uint64_t) duration, (uint64_t) waitAfter, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_get_screen_size(uint16_t * width, uint16_t * height) {
	return sys_call((uint64_t) 9, (uint64_t) width, (uint64_t) height, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_toggle_cursor() {
	return sys_call((uint64_t) 10, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_get_ticks(uint32_t * ticks) {
	return sys_call((uint64_t) 11, (uint64_t) ticks, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_write_place(unsigned int fd, const char * buffer, unsigned int size, int x, int y) {
	return sys_call((uint64_t) 12, (uint64_t) fd, (uint64_t) buffer, (uint64_t) size, (uint64_t) x, (uint64_t) y);
}

uint64_t sys_draw_image(const unsigned long * image, int width, int height) {
	return sys_call((uint64_t) 13, (uint64_t) image, (uint64_t) width, (uint64_t) height, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_create_process(char * name, char ** args, void * code, int fg, int * fds) {
	return sys_call((uint64_t) 14, (uint64_t) name, (uint64_t) args, (uint64_t) code, (uint64_t) fg, (uint64_t) fds);
}

uint64_t sys_wait_pid(uint64_t pid) {
	return sys_call((uint64_t) 15, (uint64_t) pid, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_exit(uint64_t code) {
	return sys_call((uint64_t) 16, (uint64_t) code, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

MemoryDataPtr sys_mem_data() {
	return (MemoryDataPtr) sys_call(
	    (uint64_t) 17, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

ProcessInfoPtr * sys_processes_info() {
	return (ProcessInfoPtr *) sys_call(
	    (uint64_t) 18, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

uint64_t sys_kill_process(uint64_t pid) {
	return sys_call((uint64_t) 19, (uint64_t) pid, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_block_process(uint64_t pid) {
	return sys_call((uint64_t) 20, (uint64_t) pid, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_unblock_process(uint64_t pid) {
	return sys_call((uint64_t) 21, (uint64_t) pid, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_yield() {
	return sys_call((uint64_t) 22, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_change_priority(uint64_t pid, uint64_t newPriority) {
	return sys_call((uint64_t) 23, (uint64_t) pid, (uint64_t) newPriority, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

void * sys_malloc(uint64_t size) {
	return (void *) sys_call((uint64_t) 24, (uint64_t) size, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_free(void * ptr) {
	return sys_call((uint64_t) 25, (uint64_t) ptr, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_sem_open(char * semName) {
	return sys_call((uint64_t) 26, (uint64_t) semName, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_sem_wait(int semId) {
	return sys_call((uint64_t) 27, (uint64_t) semId, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_sem_post(int semId) {
	return sys_call((uint64_t) 28, (uint64_t) semId, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_sem_close(int semId) {
	return sys_call((uint64_t) 29, (uint64_t) semId, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int sys_sem_create(char * semName, int initialValue) {
	return (int) sys_call(
	    (uint64_t) 30, (uint64_t) semName, (uint64_t) initialValue, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_sem_delete(int semId) {
	return sys_call((uint64_t) 31, (uint64_t) semId, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}

int64_t sys_get_pid() {
	return sys_call((uint64_t) 32, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0, (uint64_t) 0);
}