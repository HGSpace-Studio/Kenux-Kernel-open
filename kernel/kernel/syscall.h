/*
 * Kenux Kernel Open v26.7.9O — syscall.h
 * Copyright (c) 2026 HGSpace Studio. MIT License.
 * Developer: happygray110
 *
 * [HG-SYSCALL-H] 系统调用核心层头文件。
 *                  定义本地扩展表结构和入口函数声明。
 *                  KAPI 系统调用的完整定义在 kapi_syscall.h 中。
 */

#ifndef SYSCALL_H  /* [HG-GUARD] 防重复包含 */
#define SYSCALL_H

#include <arch/types.h>  /* [HG-TYPES] uint64_t/int64_t 等 x86_64 类型定义 */

/* [HG-SYSCALL-MAX] 本地扩展表大小。KAPI 表覆盖 0-999，这里只留 64 个动态槽位 */
#define SYSCALL_MAX 64

/*
 * [HG-SYSENT] 系统调用表项结构。
 *              handler 为 NULL 时表示该号未注册，kenux_syscall_entry 返回 -ENOSYS。
 */
typedef struct {
    void* handler;  /* [HG] 函数指针：long fn(long, long, long, long, long, long) */
} syscall_entry_t;

/* [HG-SYS-INIT] 初始化系统调用表，由 kernel_main() 在 process_init() 之后调用 */
void syscall_init(void);

/* [HG-SYS-REG] 动态注册 syscall handler 到本地扩展表 */
void syscall_register(int num, void* handler);

/* [HG-SYS-DISP] 内部分发入口（未使用，保留接口） */
void syscall_dispatch(void);

/*
 * [HG-SYS-ENTRY] SYSCALL 指令的总入口函数。
 *                   boot.S 中 MSR_LSTAR 指向此地址。
 *                   参数：nr=系统调用号, a1-a6=六个寄存器参数
 *                   返回值：long，负数表示错误（-errno）
 */
long kenux_syscall_entry(int nr, long a1, long a2, long a3, long a4, long a5, long a6);

#endif /* SYSCALL_H */