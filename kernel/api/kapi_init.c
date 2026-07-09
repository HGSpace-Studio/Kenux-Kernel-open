#include "kapi.h"

extern int kapi_process_init(void);
extern int kapi_memory_init(void);
extern int kapi_fs_init(void);
extern int kapi_device_init(void);
extern int kapi_syscall_init(void);
extern int kapi_crc_init(void);
extern int kapi_irq_init(void);
extern int kapi_io_init(void);
extern int kapi_pci_init(void);
extern int kapi_dma_init(void);
extern int kapi_security_init(void);
extern int kapi_skbuff_init(void);
extern int kapi_netdevice_init(void);
extern int kapi_socket_init(void);
extern int kapi_netlink_init(void);
extern int kapi_vfs_init(void);
extern int kapi_seq_file_init(void);
extern int kapi_debugfs_init(void);
extern int kapi_mempool_init(void);
extern int kapi_ftrace_init(void);

int kapi_init(void)
{
    int ret;

    ret = kapi_process_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_memory_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_fs_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_device_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_syscall_init();
    if (ret != KAPI_OK) return ret;

    ret = kapi_crc_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_irq_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_io_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_pci_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_dma_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_security_init();
    if (ret != KAPI_OK) return ret;

    ret = kapi_skbuff_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_netdevice_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_socket_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_netlink_init();
    if (ret != KAPI_OK) return ret;

    ret = kapi_vfs_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_seq_file_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_debugfs_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_mempool_init();
    if (ret != KAPI_OK) return ret;
    ret = kapi_ftrace_init();
    if (ret != KAPI_OK) return ret;

    return KAPI_OK;
}
