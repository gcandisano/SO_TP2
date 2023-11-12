#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <clock.h>
#include <defs.h>
#include <keyboard.h>
#include <mm_manager.h>
#include <pipes.h>
#include <process.h>
#include <sound.h>
#include <stdint.h>
#include <sync.h>
#include <syscalls.h>
#include <time.h>
#include <videodriver.h>

int64_t syscallHandler(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4);

static int64_t sys_read(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t shouldNotBlock);

static int64_t sys_write(uint64_t fd, uint64_t buffer, uint64_t length);

static int64_t sys_write_color(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t color);

static int64_t sys_write_place(uint64_t fd, uint64_t buffer, uint64_t length, uint64_t x, uint64_t y);

static int64_t sys_get_registers(uint64_t regsBuff);

static int64_t sys_get_time(uint64_t buffer);

static int64_t sys_get_date(uint64_t buffer);

static int64_t sys_clear_screen();

static int64_t sys_draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color);

static int64_t sys_play_sound(uint64_t freq, uint64_t duration, uint64_t waitAfter);

static int64_t sys_get_screensize(uint64_t width, uint64_t height);

static int64_t sys_toggle_cursor();

static int64_t sys_get_ticks(uint64_t ticks);

static int64_t sys_draw_image(uint64_t image, uint64_t width, uint64_t height);

static int64_t sys_create_process(uint64_t name, uint64_t args, uint64_t code, uint64_t fg, uint64_t fds);

static int64_t sys_wait_pid(uint64_t pid);

static int64_t sys_exit(uint64_t code);

static int64_t sys_mem_data();

static int64_t sys_processes_info();

static int64_t sys_kill_process(uint64_t pid);

static int64_t sys_block_process(uint64_t pid);

static int64_t sys_unblock_process(uint64_t pid);

static int64_t sys_yield();

static int64_t sys_change_priority(uint64_t pid, uint64_t newPriority);

static int64_t sys_malloc(uint64_t size);

static int64_t sys_free(uint64_t ptr);

static int64_t sys_sem_open(uint64_t semName);

static int64_t sys_sem_wait(uint64_t semId);

static int64_t sys_sem_post(uint64_t semId);

static int64_t sys_sem_close(uint64_t semId);

static int64_t sys_sem_create(uint64_t semName, uint64_t initialValue);

static int64_t sys_sem_delete(uint64_t semId);

static int64_t sys_get_pid();

static int64_t sys_create_anon_pipe();

static int64_t sys_wait(uint64_t delta);

static int64_t sys_create_anon_sem(uint64_t initialValue);

static int64_t sys_sem_anon_open(uint64_t semId);

#endif