// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <syscalls.h>

extern const uint64_t registers[17];

int64_t syscallHandler(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    switch(id) {
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
    }
    return -1;
} 


static int64_t sys_read(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t shouldNotBlock) {
    if (fd != STDIN) 
        return -1;
    int i = 0;
    char c;
    char * buff = (char *) buffer;
    while(i < length && (c = (shouldNotBlock ? getCharNoBlock() : getChar())) != 0) {
        buff[i] = c;
        i++;
    }
    return i;
}

static int64_t sys_write(uint64_t fd, uint64_t buffer, uint64_t length) {
    if (fd == STDOUT) {
        printStringN((char *) buffer, length);
    } else if (fd == STDERR) {
        printStringNColor((char *) buffer, length, RED);
    }
    return length;
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
    if (fd == STDOUT || fd == STDERR) {
        Color c;
        c.r = (char) color;
        c.g = (char) (color >> 8);
        c.b = (char) (color >> 16);
        printStringNColor((char *) buffer, length, c);
    }
    return length;
}

static int64_t sys_get_registers(uint64_t regsBuff) {
    for(int i = 0; i < 17; i++) {
        ((uint64_t *)regsBuff)[i] = registers[i];
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
    drawRect( (int) x, (int) y, (int) width, (int) height, (int) color );
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
    return (int64_t) createProcess((char *)name, -1, 4096, (char **)args, (void *)code, (char)fg, (int *)fds);
}

static int64_t sys_wait_pid(uint64_t pid) {
    return (int64_t) waitpid((int)pid);
}

static int64_t sys_exit(uint64_t code) {
    return killProcess((int)getCurrentPID());
}

static int64_t sys_mem_data() {
    return (int64_t) getMemoryData();
}

static int64_t sys_processes_info() {
    return (int64_t) getProcessesInfo();
}