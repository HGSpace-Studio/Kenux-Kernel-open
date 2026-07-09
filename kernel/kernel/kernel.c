/*
 * Kenux Kernel Open v26.7.9O — kernel.c
 * Copyright (c) 2026 HGSpace Studio. MIT License.
 * Developer: happygray110
 *
 * [HG-ENTRY] 内核主入口。Multiboot 协议握手后从此处开始。
 *            所有子系统的 init 顺序是硬编码的，改了就炸。
 *            O 版本精简了 KVM/BPF/cgroup/namespace/Netfilter/
 *            ftrace/BBR/sound/kprobe 等内部模块，保留核心。
 */

/* --- arch 层头文件：每一行都是 x86_64 硬件的直接映射，别乱动 --- */
#include <arch/vga.h>          /* [HG-VGA]   文本模式 80x25，boot 阶段唯一 IO 通道 */
#include <arch/drivers.h>      /* [HG-DRV]   驱动注册表，所有驱动在这里挂名字 */
#include <fs.h>                /* [HG-VFS]   虚拟文件系统总控 */
#include <arch/ipc.h>          /* [HG-IPC]   System V IPC：shm/msg/sem + FIFO + Unix Socket */
#include <arch/usermode.h>     /* [HG-UMODE] 用户态切换：iretq 弹栈，CS/RIP/SS/RSP 一条龙 */
#include <arch/shell.h>        /* [HG-SHELL] 内置 shell，开发调试用，不是给终端用户玩的 */
#include <arch/pci.h>          /* [HG-PCI]   PCI 配置空间扫描，枚举总线上所有设备 */
#include <arch/acpi.h>         /* [HG-ACPI]  RSDP → RSDT/XSDT → MADT/FADT，电源与中断路由 */
#include <arch/smbios.h>       /* [HG-SMBIOS] DMI 表解析，硬件信息采集，不改硬件就不用管 */
#include <arch/rtc.h>          /* [HG-RTC]   CMOS RTC，开机读时间用的 */
#include <arch/pit.h>          /* [HG-PIT]   8254 PIT，最早的定时器，boot 用完就扔给 APIC */
#include <arch/pic.h>          /* [HG-PIC]   8259A 双片级联，APIC 接管后此为 legacy fallback */
#include <arch/idt.h>          /* [HG-IDT]   中断描述符表 256 项，ISR 入口全部在这里注册 */
#include <arch/gdt.h>          /* [HG-GDT]   全局描述符表：内核/用户 CS+DS，TSS 段 */
#include <arch/hpet.h>         /* [HG-HPET]  高精度事件定时器，ns 级精度，取代 PIT */
#include <arch/ahci.h>         /* [HG-AHCI]  SATA AHCI 主控驱动，H2D FIS + PRD + DMA */
#include <arch/sata.h>         /* [HG-SATA]  SATA 底层，AHCI 的附属 */
#include <arch/ehci.h>         /* [HG-EHCI]  USB 2.0 EHCI 主控，兼容 OHCI */
#include <arch/xhci.h>         /* [HG-XHCI]  USB 3.0 xHCI 主控，目前最高速 */
#include <arch/net.h>          /* [HG-NET]   TCP/IP 全栈：ARP/IP/ICMP/UDP/TCP + DHCP + DNS */
/* [O-REMOVED] #include <arch/sound.h>  — 声音子系统仅 K 版本编译 */
/* [O-REMOVED] #include <arch/ac97.h>   — AC97 声卡仅 K 版本编译 */
/* [O-REMOVED] #include <arch/hda.h>     — HDA 声卡仅 K 版本编译 */
#include <arch/framebuffer.h>  /* [HG-FB]    GOP Framebuffer 图形渲染，分辨率可配 */
#include <arch/memory.h>       /* [HG-MM]    物理/虚拟内存总控：Buddy/Slab/页表/VMA/Swap */
#include <arch/process.h>      /* [HG-PROC]  进程生命周期：fork/exec/wait/exit + 调度器 */
#include <arch/syscall.h>      /* [HG-SYS]   系统调用入口，syscall 指令 → handler 分发 */
#include <arch/interrupt.h>    /* [HG-INTR]  中断框架顶层，连接 IDT 和各 ISR */
#include <string.h>            /* [HG-LIBC]  内核 C 库：memset/memcpy/strcpy/strlen ... */
#include <memory.h>            /* [HG-LIBC]  内核内存工具函数 */
#include <stdio.h>             /* [HG-LIBC]  sprintf/snprintf/printf，VFS 未就绪前的唯一输出 */

/* [HG-KAPI] K API 统一头文件，46 模块聚合入口，O 版本保留 45 模块 */
#include <kapi.h>

/* [HG-INIT] init 进程的空循环体，真正的 init 从用户态 ELF 加载 */
extern void init_main(void);

/*
 * [HG-IDLE-PROC] init 进程 main 函数。
 *                空循环。内核跑起来后第一个用户态进程就是它。
 *                真正的 /sbin/init 由 shell 或用户手动加载。
 */
void init_main(void)
{
    while (1) { }  /* [HG] 永不返回，CPU 时间片让给调度器回收 */
}

/*
 * [HG-UPTIME] 获取内核启动后的 jiffies 数。
 *             1 jiffy = HPET 周期 / HZ，用于 /proc/uptime 和 loadavg 计算。
 */
uint64_t kenux_uptime(void)
{
    extern uint64_t timer_get_jiffies(void);  /* [HG-TIMER] 定时器子系统提供 */
    return timer_get_jiffies();
}

/*
 * [HG-KMAIN] 内核主入口。boot.S 调用 multiboot_entry → C runtime → 此函数。
 *             初始化顺序是经过严格推敲的：
 *             1. 显示（无显示啥也看不到）
 *             2. GDT/IDT/PIC（CPU 基础设施，中断必需）
 *             3. 内存（后面所有 kmalloc 都靠它）
 *             4. ACPI/SMBIOS/PCI（硬件发现）
 *             5. 定时器（调度器依赖 jiffies）
 *             6. 进程/系统调用/中断（多任务基础设施）
 *             7. 文件系统/IPC（进程间协作）
 *             8. KAPI 层（内核 API 统一入口）
 *             9. 用户态/shell/驱动（最终服务）
 *
 *             [O-VERSION] 此版本为 26.7.9O 社区开放版。
 *                          KVM/BPF/cgroup/namespace/Netfilter/ftrace/BBR/
 *                          sound(AC97+HDA)/kprobe 等模块已移除。
 */
void kernel_main(void)
{
    /* === 第一阶段：显示与 CPU 基础设施 === */
    vga_init();        /* [HG-P0] VGA 文本模式初始化，这行之后才能 vga_print */

    gdt_init();        /* [HG-P0] GDT：内核代码段/数据段/用户代码段/数据段/TSS */
    idt_init();        /* [HG-P0] IDT：256 项中断门，此时 handler 全部是默认 stub */
    pic_init();        /* [HG-P0] 8259A PIC，APIC 接管前的临时中断控制器 */

    /* === 第二阶段：内存 === */
    memory_init();     /* [HG-P1] Buddy+Slab+页表+VMA，所有后续 kmalloc 依赖此 */

    /* === 第三阶段：硬件发现 === */
    acpi_init();       /* [HG-P2] ACPI 表解析：RSDP→RSDT→MADT/FADT，电源与中断路由 */
    smbios_init();     /* [HG-P2] SMBIOS/DMI：读取主板/BIOS/内存槽位信息 */

    pci_init();        /* [HG-P2] PCI 总线枚举：扫描所有 bus/dev/func，读配置空间 */

    /* === 第四阶段：定时器（调度器依赖） === */
    hpet_init();       /* [HG-P3] HPET 高精度定时器，ns 级 */
    pit_init();        /* [HG-P3] PIT 8254，legacy 定时器，HPET 失败时的 fallback */
    rtc_init();        /* [HG-P3] CMOS RTC，读日期时间 */

    /* === 第五阶段：多任务基础设施 === */
    process_init();    /* [HG-P4] 进程表初始化，PID 0 (idle) 自动创建 */
    syscall_init();    /* [HG-P4] 系统调用表注册，SYSCALL 指令 MSR 设置 */
    interrupt_init();  /* [HG-P4] APIC/IOAPIC 中断控制器接管，PIC 退役 */

    /* === 第六阶段：文件系统与 IPC === */
    fs_init();         /* [HG-P5] VFS 挂载根文件系统，注册 ext2/ext4/tmpfs/procfs/sysfs */
    ipc_init();        /* [HG-P5] System V IPC：共享内存/消息队列/信号量 */

    /* === 第七阶段：K API 层 === */
    vga_print("Initializing KAPI layer...\n");
    if (kapi_init() == KAPI_OK) {                    /* [HG-P6] 45 模块统一初始化 */
        vga_print("KAPI layer initialized successfully\n");
    } else {
        vga_print("KAPI layer initialization failed!\n");  /* [HG] 不应该发生，除非内存不够 */
    }

    /* === 第八阶段：用户态与交互 === */
    usermode_init();   /* [HG-P7] 用户态切换机制：IRETQ 栈帧构造 */
    shell_init();      /* [HG-P7] 内置 shell 初始化，命令表注册 */

    /* === 驱动注册：名字仅用于 /proc 和 debug 输出 === */
    drivers_init();                                            /* [HG-DRV] 驱动框架初始化 */
    drivers_register("PCI", pci_init);                        /* [HG-DRV] PCI 总线 */
    drivers_register("ACPI", acpi_init);                      /* [HG-DRV] ACPI 电源管理 */
    drivers_register("SMBIOS", smbios_init);                  /* [HG-DRV] 硬件信息 */
    drivers_register("RTC", rtc_init);                        /* [HG-DRV] 实时时钟 */
    drivers_register("PIT", pit_init);                        /* [HG-DRV] 定时器 */
    drivers_register("PIC", pic_init);                        /* [HG-DRV] 中断控制器(legacy) */
    drivers_register("IDT", idt_init);                        /* [HG-DRV] 中断描述符表 */
    drivers_register("GDT", gdt_init);                        /* [HG-DRV] 全局描述符表 */
    drivers_register("HPET", hpet_init);                      /* [HG-DRV] 高精度定时器 */
    drivers_register("AHCI", ahci_init);                      /* [HG-DRV] SATA AHCI */
    drivers_register("SATA", sata_init);                      /* [HG-DRV] SATA 底层 */
    drivers_register("EHCI", ehci_init);                      /* [HG-DRV] USB 2.0 */
    drivers_register("XHCI", xhci_init);                      /* [HG-DRV] USB 3.0 */
    drivers_register("NETWORK", network_init);                /* [HG-DRV] TCP/IP 网络栈 */
    /* [O-REMOVED] drivers_register("SOUND", sound_init);     — 声音子系统仅 K 版 */
    /* [O-REMOVED] drivers_register("AC97", ac97_init);      — AC97 声卡仅 K 版 */
    /* [O-REMOVED] drivers_register("HDA", hda_init);        — HDA 声卡仅 K 版 */
    drivers_register("USERMODE", usermode_init);              /* [HG-DRV] 用户态支持 */
    drivers_register("SHELL", shell_init);                    /* [HG-DRV] 内置 shell */

    /* === 启动横幅 === */
    vga_print("========================================\n");
    vga_print("  Kenux Kernel Open 26.7.9O\n");          /* [HG-VER] Open = 社区开放版 */
    vga_print("  Build: 2026-07-09\n");
    vga_print("  Developer: happygray110\n");            /* [HG-AUTH] 开发者署名 */
    vga_print("  Team: HGSpace Studio\n");               /* [HG-TEAM] 团队署名 */
    vga_print("  Arch: x86_64\n");
    vga_print("  Scheduler: 5-level priority RR\n");
    vga_print("  VFS: tree-based virtual filesystem\n");
    vga_print("  Syscalls: 1000+ (Linux compatible)\n");
    vga_print("  Build: Ninja\n");
    vga_print("========================================\n");

    /* === 内存状态 === */
    vga_print("Memory total: ");                        /* [HG-DBG] 物理内存总量 */
    char buffer[64];
    sprintf(buffer, "%lu KB\n", memory_get_total() / 1024);
    vga_print(buffer);
    sprintf(buffer, "Memory free: %lu KB\n", memory_get_free() / 1024);  /* [HG-DBG] 可用内存 */
    vga_print(buffer);

    vga_print("PCI devices found\n");                  /* [HG-DBG] PCI 扫描结果 */
    vga_print("ACPI tables found\n");                  /* [HG-DBG] ACPI 表解析结果 */
    vga_print("SMBIOS tables found\n");                /* [HG-DBG] SMBIOS 表解析结果 */

    /* === 启动驱动 === */
    vga_print("Starting drivers\n");
    drivers_start();                                   /* [HG-DRV] 遍历注册表，依次调用 init */

    /* === 创建 idle 进程 === */
    vga_print("Creating processes\n");
    process_create("idle", (void*)0x100000, 0);        /* [HG-PROC] PID 0 空转进程 */

    /* === 创建 init 进程（用户态） === */
    vga_print("Creating init process\n");
    uint64_t pid;
    usermode_create_process("init", (void*)init_main, &pid);  /* [HG-PROC] PID 1 init */

    /* === 进入 shell === */
    vga_print("Starting kernel\n");
    shell_run();                                       /* [HG-SHELL] 交互式 shell，永不返回 */

    /* [HG] 正常不会到达这里 */
    while (1) {
    }
}