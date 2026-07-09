# Kenux Kernel Open 26.7.9O - Makefile
# Developer: happygray110 | Team: HGSpace Studio

CC = gcc
AS = nasm
LD = ld

CFLAGS = -m64 -mcmodel=large -ffreestanding -fno-pic -nostdlib -nostartfiles -nodefaultlibs
ASFLAGS = -f elf64
INCLUDES = -Iinclude -Ikernel/include -Ikernel/kernel -Ikernel/arch/x86_64/include -Ikernel/lib/libc/include
LINKER_SCRIPT = kernel/arch/x86_64/boot/linker.ld

.PHONY: all clean

all: kernel.elf

kernel.elf: build/boot.o build/devtmpfs.o build/drivers.o build/epoll.o build/fifo.o build/fs.o build/gdb_stub.o build/interrupt.o build/ipc.o build/kernel.o build/kthread.o build/module.o build/printk.o build/process.o build/rcu.o build/shell_enhanced.o build/kernel_signal.o build/stacktrace.o build/sync.o build/syscall.o build/sysfs.o build/timer.o build/unixsock.o build/wait.o build/workqueue.o build/acpi.o build/acpi_madt.o build/acpi_pm.o build/ahci.o build/apic.o build/buddy.o build/cfs.o build/efi.o build/efi_entry.o build/ehci.o build/elf.o build/ext2.o build/framebuffer.o build/gdt.o build/hpet.o build/idt.o build/ioapic.o build/jbd2.o build/keyboard.o build/ldso.o build/memory.o build/mmap.o build/net.o build/nvme.o build/pagecache.o build/pagefault.o build/pci.o build/pic.o build/pit.o build/procfs.o build/rtc.o build/sata.o build/security.o build/slab.o build/smbios.o build/smp.o build/swap.o build/tmpfs.o build/usb_hid.o build/usermode.o build/vga.o build/virtio.o build/xhci.o build/kapi_device.o build/kapi_fs.o build/kapi_init.o build/kapi_memory.o build/kapi_process.o build/kapi_syscall.o build/kapi_atomic.o build/kapi_bitmap.o build/kapi_blkdev.o build/kapi_cdev.o build/kapi_completion.o build/kapi_cpumask.o build/kapi_crc.o build/kapi_debugfs.o build/kapi_dma.o build/kapi_hash.o build/kapi_idr.o build/kapi_io.o build/kapi_irq.o build/kapi_kfifo.o build/kapi_kobject.o build/kapi_kthread.o build/kapi_list.o build/kapi_mempool.o build/kapi_module.o build/kapi_mutex.o build/kapi_netdevice.o build/kapi_netlink.o build/kapi_notifier.o build/kapi_params.o build/kapi_pci.o build/kapi_random.o build/kapi_rbtree.o build/kapi_rcu.o build/kapi_sched.o build/kapi_security.o build/kapi_seq_file.o build/kapi_skbuff.o build/kapi_smp.o build/kapi_socket.o build/kapi_sort.o build/kapi_string.o build/kapi_time.o build/kapi_vfs.o build/kapi_wait.o build/errno.o build/math.o build/setjmp.o build/signal.o build/stdio.o build/stdlib.o build/string.o build/time.o build/unistd.o build/vsprintf.o build/mouse.o build/i2c.o build/spi.o build/efi_runtime.o build/watchdog.o
	$(LD) -T $(LINKER_SCRIPT) -o $@ $^

build/boot.o: kernel/arch/x86_64/boot/boot.S
	@mkdir -p build
	$(AS) $(ASFLAGS) $< -o $@

build/devtmpfs.o: kernel/kernel/devtmpfs.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/drivers.o: kernel/kernel/drivers.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/epoll.o: kernel/kernel/epoll.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/fifo.o: kernel/kernel/fifo.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/fs.o: kernel/kernel/fs.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/gdb_stub.o: kernel/kernel/gdb_stub.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/interrupt.o: kernel/kernel/interrupt.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/ipc.o: kernel/kernel/ipc.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kernel.o: kernel/kernel/kernel.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kthread.o: kernel/kernel/kthread.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/module.o: kernel/kernel/module.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/printk.o: kernel/kernel/printk.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/process.o: kernel/kernel/process.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/rcu.o: kernel/kernel/rcu.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/shell_enhanced.o: kernel/kernel/shell_enhanced.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kernel_signal.o: kernel/kernel/signal.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/stacktrace.o: kernel/kernel/stacktrace.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/sync.o: kernel/kernel/sync.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/syscall.o: kernel/kernel/syscall.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/sysfs.o: kernel/kernel/sysfs.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/timer.o: kernel/kernel/timer.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/unixsock.o: kernel/kernel/unixsock.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/wait.o: kernel/kernel/wait.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/workqueue.o: kernel/kernel/workqueue.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# [O-REMOVED] build/ac97.o: kernel/arch/x86_64/ac97.c
# 	@mkdir -p build
# 	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/acpi.o: kernel/arch/x86_64/acpi.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/acpi_madt.o: kernel/arch/x86_64/acpi_madt.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/acpi_pm.o: kernel/arch/x86_64/acpi_pm.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/ahci.o: kernel/arch/x86_64/ahci.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/apic.o: kernel/arch/x86_64/apic.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/buddy.o: kernel/arch/x86_64/buddy.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/cfs.o: kernel/arch/x86_64/cfs.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/efi.o: kernel/arch/x86_64/efi.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/efi_entry.o: kernel/arch/x86_64/efi_entry.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/ehci.o: kernel/arch/x86_64/ehci.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/elf.o: kernel/arch/x86_64/elf.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/ext2.o: kernel/arch/x86_64/ext2.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/framebuffer.o: kernel/arch/x86_64/framebuffer.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/gdt.o: kernel/arch/x86_64/gdt.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# [O-REMOVED] build/hda.o: kernel/arch/x86_64/hda.c
# 	@mkdir -p build
# 	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/hpet.o: kernel/arch/x86_64/hpet.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/idt.o: kernel/arch/x86_64/idt.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/ioapic.o: kernel/arch/x86_64/ioapic.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/jbd2.o: kernel/arch/x86_64/jbd2.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/keyboard.o: kernel/arch/x86_64/keyboard.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/ldso.o: kernel/arch/x86_64/ldso.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/memory.o: kernel/arch/x86_64/memory.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/mmap.o: kernel/arch/x86_64/mmap.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/net.o: kernel/arch/x86_64/net.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/nvme.o: kernel/arch/x86_64/nvme.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/pagecache.o: kernel/arch/x86_64/pagecache.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/pagefault.o: kernel/arch/x86_64/pagefault.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/pci.o: kernel/arch/x86_64/pci.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/pic.o: kernel/arch/x86_64/pic.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/pit.o: kernel/arch/x86_64/pit.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/procfs.o: kernel/arch/x86_64/procfs.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/rtc.o: kernel/arch/x86_64/rtc.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/sata.o: kernel/arch/x86_64/sata.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/security.o: kernel/arch/x86_64/security.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/slab.o: kernel/arch/x86_64/slab.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/smbios.o: kernel/arch/x86_64/smbios.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/smp.o: kernel/arch/x86_64/smp.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# [O-REMOVED] build/sound.o: kernel/arch/x86_64/sound.c
# 	@mkdir -p build
# 	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/swap.o: kernel/arch/x86_64/swap.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/tmpfs.o: kernel/arch/x86_64/tmpfs.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/usb_hid.o: kernel/arch/x86_64/usb_hid.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/usermode.o: kernel/arch/x86_64/usermode.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/vga.o: kernel/arch/x86_64/vga.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/virtio.o: kernel/arch/x86_64/virtio.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/xhci.o: kernel/arch/x86_64/xhci.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_device.o: kernel/api/kapi_device.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_fs.o: kernel/api/kapi_fs.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_init.o: kernel/api/kapi_init.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_memory.o: kernel/api/kapi_memory.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_process.o: kernel/api/kapi_process.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_syscall.o: kernel/api/kapi_syscall.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_atomic.o: kernel/api/kapi_atomic.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_bitmap.o: kernel/api/kapi_bitmap.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_blkdev.o: kernel/api/kapi_blkdev.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_cdev.o: kernel/api/kapi_cdev.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_completion.o: kernel/api/kapi_completion.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_cpumask.o: kernel/api/kapi_cpumask.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_crc.o: kernel/api/kapi_crc.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_debugfs.o: kernel/api/kapi_debugfs.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_dma.o: kernel/api/kapi_dma.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# [O-REMOVED] build/kapi_ftrace.o: kernel/api/kapi_ftrace.c
# 	@mkdir -p build
# 	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_hash.o: kernel/api/kapi_hash.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_idr.o: kernel/api/kapi_idr.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_io.o: kernel/api/kapi_io.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_irq.o: kernel/api/kapi_irq.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_kfifo.o: kernel/api/kapi_kfifo.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_kobject.o: kernel/api/kapi_kobject.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_kthread.o: kernel/api/kapi_kthread.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_list.o: kernel/api/kapi_list.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_mempool.o: kernel/api/kapi_mempool.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_module.o: kernel/api/kapi_module.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_mutex.o: kernel/api/kapi_mutex.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_netdevice.o: kernel/api/kapi_netdevice.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_netlink.o: kernel/api/kapi_netlink.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_notifier.o: kernel/api/kapi_notifier.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_params.o: kernel/api/kapi_params.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_pci.o: kernel/api/kapi_pci.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_random.o: kernel/api/kapi_random.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_rbtree.o: kernel/api/kapi_rbtree.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_rcu.o: kernel/api/kapi_rcu.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_sched.o: kernel/api/kapi_sched.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_security.o: kernel/api/kapi_security.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_seq_file.o: kernel/api/kapi_seq_file.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_skbuff.o: kernel/api/kapi_skbuff.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_smp.o: kernel/api/kapi_smp.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_socket.o: kernel/api/kapi_socket.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_sort.o: kernel/api/kapi_sort.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_string.o: kernel/api/kapi_string.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_time.o: kernel/api/kapi_time.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_vfs.o: kernel/api/kapi_vfs.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/kapi_wait.o: kernel/api/kapi_wait.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/errno.o: kernel/lib/libc/errno.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/math.o: kernel/lib/libc/math.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/setjmp.o: kernel/lib/libc/setjmp.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/signal.o: kernel/lib/libc/signal.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/stdio.o: kernel/lib/libc/stdio.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/stdlib.o: kernel/lib/libc/stdlib.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/string.o: kernel/lib/libc/string.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/time.o: kernel/lib/libc/time.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/unistd.o: kernel/lib/libc/unistd.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/vsprintf.o: kernel/lib/libc/vsprintf.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/mouse.o: kernel/arch/x86_64/mouse.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/i2c.o: kernel/arch/x86_64/i2c.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/spi.o: kernel/arch/x86_64/spi.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/efi_runtime.o: kernel/arch/x86_64/efi_runtime.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

build/watchdog.o: kernel/arch/x86_64/watchdog.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf build kernel.elf
