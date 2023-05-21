#ifndef SYSCALLS_H
#define SYSCALLS_H

void syscallHandler(uint64_t id, uint64_t arg0, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5);

static int64_t sys_read(uint64_t fd, uint64_t buffer, uint64_t length);

static void sys_write(uint64_t fd, uint64_t buffer, uint64_t length);

#endif