// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>

extern const uint64_t registers[17];

int64_t syscallHandler(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
	switch (id) {
		case 0:
			return (int64_t) sys_read(arg0, arg1, arg2, arg3);
		case 1:
			return (int64_t) sys_write(arg0, arg1, arg2);
		case 2:
			return (int64_t) sys_write_color(arg0, arg1, arg2, arg3);
		case 3:
			return (int64_t) sys_get_registers(arg0);
		case 4:
			return (int64_t) sys_get_time(arg0);
		case 5:
			return (int64_t) sys_get_date(arg0);
		case 6:
			return (int64_t) sys_clear_screen();
		case 7:
			return (int64_t) sys_draw_rect(arg0, arg1, arg2, arg3, arg4);
		case 8:
			return (int64_t) sys_play_sound(arg0, arg1, arg2);
		case 9:
			return (int64_t) sys_get_screensize(arg0, arg1);
		case 10:
			return (int64_t) sys_toggle_cursor();
		case 11:
			return (int64_t) sys_get_ticks(arg0);
		case 12:
			return (int64_t) sys_write_place(arg0, arg1, arg2, arg3, arg4);
		case 13:
			return (int64_t) sys_draw_image(arg0, arg1, arg2);
		case 14:
			return (int64_t) sys_create_process(arg0, arg1, arg2, arg3, arg4);
		case 15:
			return (int64_t) sys_wait_pid(arg0);
		case 16:
			return (int64_t) sys_exit(arg0);
		case 17:
			return (int64_t) sys_mem_data();
		case 18:
			return (int64_t) sys_processes_info();
		case 19:
			return (int64_t) sys_kill_process(arg0);
		case 20:
			return (int64_t) sys_block_process(arg0);
		case 21:
			return (int64_t) sys_unblock_process(arg0);
		case 22:
			return (int64_t) sys_yield();
		case 23:
			return (int64_t) sys_change_priority(arg0, arg1);
		case 24:
			return (int64_t) sys_malloc(arg0);
		case 25:
			return (int64_t) sys_free(arg0);
		case 26:
			return (int64_t) sys_sem_open(arg0);
		case 27:
			return (int64_t) sys_sem_wait(arg0);
		case 28:
			return (int64_t) sys_sem_post(arg0);
		case 29:
			return (int64_t) sys_sem_close(arg0);
		case 30:
			return (int64_t) sys_sem_create(arg0, arg1);
		case 31:
			return (int64_t) sys_sem_delete(arg0);
		case 32:
			return (int64_t) sys_get_pid();
		case 33:
			return (int64_t) sys_create_anon_pipe();
		case 34:
			return (int64_t) sys_wait(arg0);
		case 35:
			return (int64_t) sys_create_anon_sem(arg0);
	}
	return -1;
}

static int64_t sys_read(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t shouldNotBlock) {
	if (fd != STDIN)
		return -1;
	PCB * currentProcess = getCurrentPCB();
	if (currentProcess->fd[STDIN] == STDIN) {
		int i = 0;
		char c;
		char * buff = (char *) buffer;
		while (i < length && (c = (shouldNotBlock ? getCharNoBlock() : getChar())) != 0) {
			buff[i] = c;
			i++;
		}
		return i;
	} else {
		return readPipe(currentProcess->fd[STDIN], (char *) buffer, length);
	}
}

static int64_t sys_write(uint64_t fd, uint64_t buffer, uint64_t length) {
	return sys_write_color(fd, buffer, length, 0xFFFFFF);
}

static int64_t sys_write_place(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t x, uint64_t y) {
	if (fd == STDOUT) {
		printStringPlace((char *) buffer, (int) x, (int) y, WHITE);
	} else if (fd == STDERR) {
		printStringPlace((char *) buffer, (int) x, (int) y, RED);
	}
	return length;
}

static int64_t sys_write_color(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t color) {
	PCB * currentProcess = getCurrentPCB();
	if (currentProcess->fd[STDOUT] == STDOUT) {
		if (fd == STDOUT) {
			Color c;
			c.r = (char) color;
			c.g = (char) (color >> 8);
			c.b = (char) (color >> 16);
			printStringNColor((char *) buffer, length, c);
		} else if (fd == STDERR) {
			printStringNColor((char *) buffer, length, RED);
		}
	} else {
		return writePipe(currentProcess->fd[STDOUT], (char *) buffer, length);
	}
	return length;
}

static int64_t sys_get_registers(uint64_t regsBuff) {
	for (int i = 0; i < 17; i++) {
		((uint64_t *) regsBuff)[i] = registers[i];
	}
	return 0;
}

static int64_t sys_get_time(uint64_t buffer) {
	timeToStr((char *) buffer);
	return 0;
}

static int64_t sys_get_date(uint64_t buffer) {
	dateToStr((char *) buffer);
	return 0;
}

static int64_t sys_clear_screen() {
	clearScreen();
	return 0;
}

static int64_t sys_draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color) {
	drawRect((int) x, (int) y, (int) width, (int) height, (int) color);
	return 0;
}

static int64_t sys_play_sound(uint64_t freq, uint64_t duration, uint64_t waitAfter) {
	playNote((int) freq, (int) duration, (int) waitAfter);
	return 0;
}

static int64_t sys_get_screensize(uint64_t width, uint64_t height) {
	uint16_t * w = (uint16_t *) width;
	uint16_t * h = (uint16_t *) height;
	*w = getWidth();
	*h = getHeight();
	return 0;
}

static int64_t sys_toggle_cursor() {
	toggleCursor();
	return 0;
}

static int64_t sys_get_ticks(uint64_t ticks) {
	uint32_t * t = (uint32_t *) ticks;
	*t = ticks_elapsed();
	return (int64_t) *t;
}

static int64_t sys_draw_image(uint64_t image, uint64_t width, uint64_t height) {
	drawImage((const unsigned long int *) image, (int) width, (int) height);
	return 0;
}

static int64_t sys_create_process(uint64_t name, uint64_t args, uint64_t code, uint64_t fg, uint64_t fds) {
	return (int64_t) createProcess((char *) name, -1, 4096, (char **) args, (void *) code, (char) fg, (int *) fds);
}

static int64_t sys_wait_pid(uint64_t pid) {
	return (int64_t) waitpid((int) pid);
}

static int64_t sys_exit(uint64_t code) {
	return killProcess((int) getCurrentPID());
}

static int64_t sys_mem_data() {
	return (int64_t) getMemoryData();
}

static int64_t sys_processes_info() {
	return (int64_t) getProcessesInfo();
}

static int64_t sys_kill_process(uint64_t pid) {
	return (int64_t) killProcess(pid);
}

static int64_t sys_block_process(uint64_t pid) {
	return (int64_t) blockProcess(pid);
}

static int64_t sys_unblock_process(uint64_t pid) {
	return (int64_t) unblockProcess(pid);
}

static int64_t sys_yield() {
	yield();
	return 0;
}

static int64_t sys_change_priority(uint64_t pid, uint64_t newPriority) {
	return (int64_t) changePriority(pid, newPriority);
}

static int64_t sys_malloc(uint64_t size) {
	return (int64_t) malloc((int) size);
}

static int64_t sys_free(uint64_t ptr) {
	free((void *) ptr);
	return 0;
}

static int64_t sys_sem_open(uint64_t semName) {
	return (int64_t) semOpen((char *) semName);
}

static int64_t sys_sem_wait(uint64_t semId) {
	return (int64_t) semWait((int) semId);
}

static int64_t sys_sem_post(uint64_t semId) {
	return (int64_t) semPost((int) semId);
}

static int64_t sys_sem_close(uint64_t semId) {
	return (int64_t) semClose((int) semId);
}

static int64_t sys_sem_create(uint64_t semName, uint64_t initialValue) {
	return (int64_t) semCreate((char *) semName, (int) initialValue);
}

static int64_t sys_sem_delete(uint64_t semId) {
	return (int64_t) semDestroy((int) semId);
}

static int64_t sys_get_pid() {
	return (int64_t) getCurrentPID();
}

static int64_t sys_create_anon_pipe() {
	return (int64_t) createAnonPipe();
}

static int64_t sys_wait(uint64_t delta) {
	timer_wait(delta);
	return 0;
}

static int64_t sys_create_anon_sem(uint64_t initialValue) {
	return (int64_t) semCreateAnon((int) initialValue);
}