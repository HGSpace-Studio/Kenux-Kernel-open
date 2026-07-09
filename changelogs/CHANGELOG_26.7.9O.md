# Kenux Kernel Open v26.7.9O 更新日志

**发布日期：** 2026-07-09

**主题：** 社区开放版 — 精简高级模块，面向社区发布

**开发者：** happygray110 | **团队：** HGSpace Studio

---

## 总览

本版本是 Kenux Kernel Open 首个面向社区的开放版本（O = Open）。基于内部版本 26.7.9K，移除了仅限内部使用的高级模块，保留社区开发、软件测试、驱动移植所需的核心功能。

---

## 版本策略

| 后缀 | 含义 | 说明 |
|------|------|------|
| O | Open | 社区开放版本，MIT 许可证，可自由使用和贡献 |
| K | 内部版 | HGSpace Studio 内部开发版本，不公开发布 |

---

## 移除的模块

以下模块仅存在于内部 K 版本，已从 O 版本中移除：

### 虚拟化与容器
- `kvm.c` / `kvm_vmx.c` — KVM / VMX 虚拟化
- `cgroup.c` — 控制组
- `namespace.c` — 命名空间

### 网络过滤
- `netfilter.c` / `conntrack.c` — Netfilter 防火墙 / 连接跟踪
- `bpf.c` — BPF 字节码过滤

### 网络拥塞
- `tcp_bbr.c` — BBR 拥塞控制（保留 Reno / CUBIC）

### 跟踪与审计
- `ftrace.c` — 函数跟踪
- `kapi_ftrace.c` — 函数跟踪 K API

### 设备驱动
- `ac97.c` / `hda.c` — 声卡驱动
- `sound.c` — 声音子系统

---

## 保留的核心模块

### 进程与调度
- 5 级优先级轮转调度 + CFS 完全公平调度器
- kthread 内核线程、用户态 ELF 加载器 + ld.so

### 内存管理
- Buddy / Slab / 四级页表 / VMA / Page Cache / Swap

### 同步与 IPC
- 自旋锁 / 互斥锁 / 读写锁 / 信号量 / RCU / System V IPC / Futex / FIFO / Unix Socket

### 安全
- LSM 可加载安全模块框架
- SELinux 强制访问控制
- security.c 基础安全子系统
- kapi_security.c 安全 API

### 文件系统
- VFS + ext2 + ext4（JBD2 日志） + TMPFS / procfs / sysfs / devtmpfs

### 网络
- TCP/IP 全协议栈 / Reno + CUBIC 拥塞控制 / DHCP / DNS / BSD Socket / Epoll

### 设备驱动
- AHCI SATA / NVMe / xHCI / EHCI / USB HID / PS/2 键鼠 / VGA / Framebuffer
- APIC / IOAPIC / PCI / ACPI / VirtIO

### K API
- 45 个模块（原 46 个，仅移除 ftrace）

### 调试
- GDB 远程调试 Stub / procfs

---

## 版本号变更
- 版本字符串：`26.7.9K` → `26.7.9O`
- 项目名：`Kenux Kernel` → `Kenux Kernel Open`
- 许可证：内部许可 → MIT License
- procfs 版本信息更新为 26.7.9O

---

## 构建变更
- `Makefile` 和 `build.ninja` 中移除被删模块的编译条目
- `kernel.c` 中移除被删驱动的初始化和注册调用

---

## 许可证

MIT License — Copyright (c) 2026 HGSpace Studio