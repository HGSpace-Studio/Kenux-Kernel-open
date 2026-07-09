/*
 * Kenux Kernel Open v26.7.9O — syscall.c
 * Copyright (c) 2026 HGSpace Studio. MIT License.
 * Developer: happygray110
 *
 * [HG-SYSCALL-CORE] 系统调用核心调度层。
 *                   两级分发：KAPI 系统调用表优先，本地表兜底。
 *                   SYSCALL 指令触发 → MSR_LSTAR → 此函数。
 *                   返回 -38 = -ENOSYS（功能未实现）。
 */

#include "syscall.h"
#include <kapi_syscall.h>    /* [HG-SYS] KAPI 系统调用定义，包含全部 1000+ 号 */
#include <memory.h>           /* [HG-LIBC] kzalloc/kfree */
#include <string.h>           /* [HG-LIBC] memset */

/* [HG-SYSTBL] 本地系统调用表，SYSCALL_MAX=64 项。KAPI 表覆盖 0-999，此表为扩展预留 */
static syscall_entry_t syscall_table[SYSCALL_MAX];

/*
 * [HG-SYS-INIT] 系统调用表初始化。
 *                先清零本地表，再调用 KAPI 层注册全部 syscall handler。
 *                必须在 kernel_main 中 process_init() 之后调用。
 */
void syscall_init(void)
{
    memset(syscall_table, 0, sizeof(syscall_table));  /* [HG] 清零，handler=NULL 表示 ENOSYS */

    kapi_syscall_init();  /* [HG-KAPI] KAPI 层 syscall 注册，覆盖 read/write/mmap/... 全部 POSIX */
}

/*
 * [HG-SYS-REG] 动态注册系统调用处理函数到本地扩展表。
 *              超出 SYSCALL_MAX 的号直接丢弃，不会越界。
 */
void syscall_register(int num, void* handler)
{
    if (num >= SYSCALL_MAX) {  /* [HG-BOUNDS] 防越界 */
        return;
    }

    syscall_table[num].handler = handler;  /* [HG] 直接写入函数指针 */
}

/*
 * [HG-SYS-GET] 查询本地扩展表中某号 syscall 的 handler。
 */
void* syscall_get(int num)
{
    if (num >= SYSCALL_MAX) {  /* [HG-BOUNDS] 防越界 */
        return NULL;
    }

    return syscall_table[num].handler;  /* [HG] 返回函数指针，NULL=未注册 */
}

/*
 * [HG-SYS-ENTRY] 系统调用总入口。
 *                  SYSCALL 指令将用户态 6 个参数放入 rdi/rsi/rdx/r10/r8/r9，
 *                  系统调用号在 rax，CPU 自动切到内核栈后跳转到这里。
 *
 *                  分发策略（优先级从高到低）：
 *                  1. KAPI 表（0 ~ KAPI_SYSCALL_COUNT-1）— 绝大多数 POSIX syscall
 *                  2. 本地扩展表（0 ~ SYSCALL_MAX-1）— 运行时动态注册的
 *                  3. 返回 -ENOSYS（-38）— 未实现的系统调用
 *
 * [O-VERSION] kprobe/trace/audit/IoMMU/GPU 相关 syscall 号在 KAPI 层保留定义但
 *              O 版本不注册 handler，调用会返回 -ENOSYS。
 */
long kenux_syscall_entry(int nr, long a1, long a2, long a3, long a4, long a5, long a6)
{
    /* [HG-PATH-1] KAPI 系统调用表：覆盖 0~999，包含全部 Linux 兼容 syscall */
    if (nr >= 0 && nr < KAPI_SYSCALL_COUNT) {
        return kapi_syscall_dispatch(nr, a1, a2, a3, a4, a5, a6);
    }

    /* [HG-PATH-2] 本地扩展表：运行时动态注册 */
    if (nr >= 0 && nr < SYSCALL_MAX && syscall_table[nr].handler) {
        kapi_syscall_fn_t fn = (kapi_syscall_fn_t)syscall_table[nr].handler;
        return fn(a1, a2, a3, a4, a5, a6);
    }

    /* [HG-PATH-3] 未实现 → -ENOSYS */
    return -38;  /* [HG] -ENOSYS = Function not implemented */
}